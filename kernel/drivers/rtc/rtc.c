/*
 * rtc.c - the CMOS real-time clock
 *
 * The MC146818 updates its registers once a second, and reading them
 * mid-update gives a torn time. The standard fix, used here, is to
 * read the whole clock twice and accept the result only when the two
 * reads agree - and to wait out the update-in-progress flag first.
 */

#include "drivers/rtc/rtc.h"

#include "drivers/pit/pit.h"
#include "../../arch/x86_64/io.h"
#include "../../core/klib.h"

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

#define RTC_SECONDS 0x00
#define RTC_MINUTES 0x02
#define RTC_HOURS   0x04
#define RTC_DAY     0x07
#define RTC_MONTH   0x08
#define RTC_YEAR    0x09
#define RTC_STATUS_A 0x0a
#define RTC_STATUS_B 0x0b

static uint64_t boot_epoch;

static uint8_t cmos_read(uint8_t reg) {
    /* Bit 7 of the address port keeps NMI masked while we read. */
    outb(CMOS_ADDR, (uint8_t)(0x80 | reg));
    return inb(CMOS_DATA);
}

static bool rtc_updating(void) {
    return (cmos_read(RTC_STATUS_A) & 0x80) != 0;
}

static uint8_t bcd_to_bin(uint8_t v) {
    return (uint8_t)((v & 0x0f) + ((v >> 4) * 10));
}

struct rtc_time {
    uint8_t second, minute, hour, day, month;
    uint16_t year;
};

static void rtc_read_raw(struct rtc_time *t) {
    while (rtc_updating()) { }

    t->second = cmos_read(RTC_SECONDS);
    t->minute = cmos_read(RTC_MINUTES);
    t->hour   = cmos_read(RTC_HOURS);
    t->day    = cmos_read(RTC_DAY);
    t->month  = cmos_read(RTC_MONTH);
    t->year   = cmos_read(RTC_YEAR);
}

/* Days from the civil epoch, Howard Hinnant's algorithm: valid for
 * every year the RTC can express, with no lookup table. */
static int64_t days_from_civil(int64_t y, unsigned m, unsigned d) {
    y -= m <= 2;
    int64_t era = (y >= 0 ? y : y - 399) / 400;
    unsigned yoe = (unsigned)(y - era * 400);
    unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + (int64_t)doe - 719468;
}

/* The inverse of days_from_civil() above - same algorithm family
 * (Howard Hinnant's civil_from_days), same validity range. */
static void civil_from_days(int64_t z, int64_t *y, unsigned *m, unsigned *d) {
    z += 719468;
    int64_t era = (z >= 0 ? z : z - 146096) / 146097;
    unsigned doe = (unsigned)(z - era * 146097);
    unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    int64_t y_ = (int64_t)yoe + era * 400;
    unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    unsigned mp = (5 * doy + 2) / 153;
    unsigned d_ = doy - (153 * mp + 2) / 5 + 1;
    unsigned m_ = mp + (mp < 10 ? 3 : (unsigned)-9);
    y_ += (m_ <= 2);
    *y = y_;
    *m = m_;
    *d = d_;
}

void rtc_civil_from_epoch(uint64_t epoch, int *year, int *month, int *day,
                          int *hour, int *minute, int *second) {
    int64_t days = (int64_t)(epoch / 86400);
    int64_t rem = (int64_t)(epoch % 86400);
    int64_t y;
    unsigned m, d;
    civil_from_days(days, &y, &m, &d);
    *year = (int)y;
    *month = (int)m;
    *day = (int)d;
    *hour = (int)(rem / 3600);
    *minute = (int)((rem % 3600) / 60);
    *second = (int)(rem % 60);
}

void rtc_init(void) {
    struct rtc_time a, b;

    /* Read until two consecutive reads agree: anything else was torn
     * by the chip's once-a-second update. */
    rtc_read_raw(&a);
    for (int i = 0; i < 10; i++) {
        rtc_read_raw(&b);
        if (a.second == b.second && a.minute == b.minute &&
            a.hour == b.hour && a.day == b.day &&
            a.month == b.month && a.year == b.year) {
            break;
        }
        a = b;
    }

    uint8_t status_b = cmos_read(RTC_STATUS_B);
    bool binary = (status_b & 0x04) != 0;
    bool hour24 = (status_b & 0x02) != 0;

    if (!binary) {
        a.second = bcd_to_bin(a.second);
        a.minute = bcd_to_bin(a.minute);
        a.day = bcd_to_bin(a.day);
        a.month = bcd_to_bin(a.month);
        a.year = bcd_to_bin((uint8_t)a.year);
        /* The 12-hour PM flag lives in bit 7 and must survive the
         * BCD conversion, so the hour is handled separately. */
        uint8_t pm = a.hour & 0x80;
        a.hour = (uint8_t)(bcd_to_bin((uint8_t)(a.hour & 0x7f)) | pm);
    }

    if (!hour24 && (a.hour & 0x80)) {
        a.hour = (uint8_t)(((a.hour & 0x7f) % 12) + 12);
    }

    /* The RTC keeps two digits. Anything below 70 is this century. */
    int year = a.year < 70 ? 2000 + a.year : 1900 + a.year;

    int64_t days = days_from_civil(year, a.month ? a.month : 1,
                                   a.day ? a.day : 1);
    boot_epoch = (uint64_t)(days * 86400 + a.hour * 3600 +
                            a.minute * 60 + a.second);

    /* Whatever the PIT has already counted is time that passed after
     * the clock we just read, so take it back off. */
    boot_epoch -= pit_uptime_ms() / 1000;

    kprintf("rtc: %04d-%02u-%02u %02u:%02u:%02u UTC\n",
            year, a.month, a.day, a.hour, a.minute, a.second);
}

uint64_t rtc_boot_epoch(void) { return boot_epoch; }

uint64_t rtc_now(void) { return boot_epoch + pit_uptime_ms() / 1000; }

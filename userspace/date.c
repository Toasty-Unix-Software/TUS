/*
 * date.c - show the wall clock, UTC (TUS port). Real /bin binary
 * version of tsh's cmd_date (kernel/shell/cmd_fs.c), via musl's
 * standard time()/gmtime() (both already route through SYS_CLOCK -
 * the CMOS RTC - see sources/musl-1.2.6/src/internal/tus_syscall.c's
 * tus_clock_gettime()), rather than reading the RTC by hand.
 */

#include <stdio.h>
#include <time.h>

int main(void) {
    time_t t = time(NULL);
    struct tm tm;
    gmtime_r(&t, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S UTC", &tm);
    printf("%s\n", buf);
    return 0;
}

/*
 * keymap.c - keyboard layouts
 *
 * See keymap.h for the model. What follows is data: one table per
 * layout, indexed by scancode set 1 make code, four levels deep.
 *
 * The tables are written as designated initialisers in scancode
 * order, with the physical row they belong to called out, because
 * that is how a keyboard is laid out and how a mistake is spotted.
 * The scancodes of the main block, for reference:
 *
 *   0x29 0x02..0x0D          the digit row, ` 1 2 3 ... - =
 *   0x10..0x1B               Q W E R T Y U I O P [ ]
 *   0x1E..0x28, 0x2B         A S D F G H J K L ; ' \
 *   0x56, 0x2C..0x35         (ISO key) Z X C V B N M , . /
 *
 * 0x56 is the extra key an ISO (European) keyboard has to the left of
 * Z. A US keyboard does not have it and never sends the code, so
 * giving it a character costs nothing and having it is what makes the
 * German < > | key work.
 */

#include "drivers/keymap/keymap.h"

#include "../../core/klib.h"

#define LEVELS 4
#define CODES  128

typedef uint16_t keytable[LEVELS][CODES];

/* ---- the keys every layout shares ----
 *
 * Space, Tab, Enter, Escape and Backspace are the same everywhere and
 * are listed once here; each layout's table is seeded from this one so
 * a layout author cannot forget them. */
#define COMMON_KEYS \
    [0x01] = 0x1B, /* Escape    */ \
    [0x0E] = '\b', /* Backspace */ \
    [0x0F] = '\t', /* Tab       */ \
    [0x1C] = '\n', /* Enter     */ \
    [0x39] = ' ',  /* Space     */ \
    [0x37] = '*',  /* keypad multiply */ \
    [0x4A] = '-',  /* keypad minus    */ \
    [0x4E] = '+'   /* keypad plus     */

/* ================= US (QWERTY) ================= */

static const keytable km_us = {
    /* plain */ {
        COMMON_KEYS,
        [0x29]='`', [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',
        [0x07]='6',[0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',
        [0x0C]='-',[0x0D]='=',
        [0x10]='q',[0x11]='w',[0x12]='e',[0x13]='r',[0x14]='t',[0x15]='y',
        [0x16]='u',[0x17]='i',[0x18]='o',[0x19]='p',[0x1A]='[',[0x1B]=']',
        [0x1E]='a',[0x1F]='s',[0x20]='d',[0x21]='f',[0x22]='g',[0x23]='h',
        [0x24]='j',[0x25]='k',[0x26]='l',[0x27]=';',[0x28]='\'',[0x2B]='\\',
        [0x2C]='z',[0x2D]='x',[0x2E]='c',[0x2F]='v',[0x30]='b',[0x31]='n',
        [0x32]='m',[0x33]=',',[0x34]='.',[0x35]='/',
    },
    /* shift */ {
        COMMON_KEYS,
        [0x29]='~', [0x02]='!',[0x03]='@',[0x04]='#',[0x05]='$',[0x06]='%',
        [0x07]='^',[0x08]='&',[0x09]='*',[0x0A]='(',[0x0B]=')',
        [0x0C]='_',[0x0D]='+',
        [0x10]='Q',[0x11]='W',[0x12]='E',[0x13]='R',[0x14]='T',[0x15]='Y',
        [0x16]='U',[0x17]='I',[0x18]='O',[0x19]='P',[0x1A]='{',[0x1B]='}',
        [0x1E]='A',[0x1F]='S',[0x20]='D',[0x21]='F',[0x22]='G',[0x23]='H',
        [0x24]='J',[0x25]='K',[0x26]='L',[0x27]=':',[0x28]='"',[0x2B]='|',
        [0x2C]='Z',[0x2D]='X',[0x2E]='C',[0x2F]='V',[0x30]='B',[0x31]='N',
        [0x32]='M',[0x33]='<',[0x34]='>',[0x35]='?',
    },
    /* altgr */ { COMMON_KEYS },
    /* shift+altgr */ { COMMON_KEYS },
};

/* ================= Turkish Q (the common one) =================
 *
 * The layout on almost every keyboard sold in Turkey. QWERTY with the
 * six Turkish letters folded in and the punctuation moved to make
 * room:
 *
 *   " 1 2 3 4 5 6 7 8 9 0 * -        (shift: e ! ' ^ + % & / ( ) = ? _)
 *   q w e r t y u i o p g u          (i is DOTLESS, g is g-breve, u is u-umlaut)
 *   a s d f g h j k l s i ,          (s is s-cedilla, i is DOTTED)
 *   z x c v b n m o c .              (o is o-umlaut, c is c-cedilla)
 *
 * The two keys worth staring at are 0x17 and 0x28. Turkish has two
 * letter i's, and they are different letters: dotless i/I (0x0131 and
 * 'I') and dotted i/İ ('i' and 0x0130). The 'i' key on a US keyboard
 * is the DOTLESS one here, and its capital is plain I; the key where
 * US has an apostrophe is the dotted one, whose capital is İ. Caps
 * Lock swapping levels rather than upper-casing is what keeps this
 * right - see keymap.h.
 */
static const keytable km_tr = {
    /* plain */ {
        COMMON_KEYS,
        [0x29]='"', [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',
        [0x07]='6',[0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',
        [0x0C]='*',[0x0D]='-',
        [0x10]='q',[0x11]='w',[0x12]='e',[0x13]='r',[0x14]='t',[0x15]='y',
        [0x16]='u',[0x17]=0x0131,[0x18]='o',[0x19]='p',
        [0x1A]=0x011F,[0x1B]=0x00FC,
        [0x1E]='a',[0x1F]='s',[0x20]='d',[0x21]='f',[0x22]='g',[0x23]='h',
        [0x24]='j',[0x25]='k',[0x26]='l',[0x27]=0x015F,[0x28]='i',[0x2B]=',',
        [0x56]='<',
        [0x2C]='z',[0x2D]='x',[0x2E]='c',[0x2F]='v',[0x30]='b',[0x31]='n',
        [0x32]='m',[0x33]=0x00F6,[0x34]=0x00E7,[0x35]='.',
    },
    /* shift */ {
        COMMON_KEYS,
        [0x29]=0x00E9, [0x02]='!',[0x03]='\'',[0x04]=KEYMAP_DK(MARK_CIRCUMFLEX),
        [0x05]='+',[0x06]='%',[0x07]='&',[0x08]='/',[0x09]='(',[0x0A]=')',
        [0x0B]='=',[0x0C]='?',[0x0D]='_',
        [0x10]='Q',[0x11]='W',[0x12]='E',[0x13]='R',[0x14]='T',[0x15]='Y',
        [0x16]='U',[0x17]='I',[0x18]='O',[0x19]='P',
        [0x1A]=0x011E,[0x1B]=0x00DC,
        [0x1E]='A',[0x1F]='S',[0x20]='D',[0x21]='F',[0x22]='G',[0x23]='H',
        [0x24]='J',[0x25]='K',[0x26]='L',[0x27]=0x015E,[0x28]=0x0130,[0x2B]=';',
        [0x56]='>',
        [0x2C]='Z',[0x2D]='X',[0x2E]='C',[0x2F]='V',[0x30]='B',[0x31]='N',
        [0x32]='M',[0x33]=0x00D6,[0x34]=0x00C7,[0x35]=':',
    },
    /* altgr */ {
        COMMON_KEYS,
        [0x29]='`', [0x02]='>',[0x03]=0x00A3,[0x04]='#',[0x05]='$',
        [0x06]=0x00BD,[0x08]='{',[0x09]='[',[0x0A]=']',[0x0B]='}',
        [0x0C]='\\',[0x0D]='|',
        [0x10]='@',[0x12]=0x20AC,
        [0x1A]=KEYMAP_DK(MARK_DIAERESIS),[0x1B]=KEYMAP_DK(MARK_TILDE),
        [0x1E]=0x00E6,[0x1F]=0x00DF,[0x27]=KEYMAP_DK(MARK_ACUTE),
        [0x56]='|',
    },
    /* shift+altgr */ {
        COMMON_KEYS,
        [0x1E]=0x00C6,
    },
};

/* ================= Turkish F =================
 *
 * The layout Turkish was actually designed for: the letter positions
 * come from the frequency of Turkish, not from an English typewriter.
 * Far less common than Q, and the reason it is here is that anyone who
 * learned to type on it cannot use a Q keyboard at all.
 *
 *   + 1 2 3 4 5 6 7 8 9 0 / -
 *   f g g i o d r n h p q w        (g is g-breve, i is DOTLESS)
 *   u i e a u t k m l y s x        (second u is u-umlaut, s is s-cedilla)
 *   j o v c c z s b . ,            (o is o-umlaut, second c is c-cedilla)
 */
static const keytable km_trf = {
    /* plain */ {
        COMMON_KEYS,
        [0x29]='+', [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',
        [0x07]='6',[0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',
        [0x0C]='/',[0x0D]='-',
        [0x10]='f',[0x11]='g',[0x12]=0x011F,[0x13]=0x0131,[0x14]='o',
        [0x15]='d',[0x16]='r',[0x17]='n',[0x18]='h',[0x19]='p',
        [0x1A]='q',[0x1B]='w',
        [0x1E]='u',[0x1F]='i',[0x20]='e',[0x21]='a',[0x22]=0x00FC,
        [0x23]='t',[0x24]='k',[0x25]='m',[0x26]='l',[0x27]='y',
        [0x28]=0x015F,[0x2B]='x',
        [0x56]='<',
        [0x2C]='j',[0x2D]=0x00F6,[0x2E]='v',[0x2F]='c',[0x30]=0x00E7,
        [0x31]='z',[0x32]='s',[0x33]='b',[0x34]='.',[0x35]=',',
    },
    /* shift */ {
        COMMON_KEYS,
        [0x29]='*', [0x02]='!',[0x03]='"',[0x04]=KEYMAP_DK(MARK_CIRCUMFLEX),
        [0x05]='$',[0x06]='%',[0x07]='&',[0x08]='\'',[0x09]='(',[0x0A]=')',
        [0x0B]='=',[0x0C]='?',[0x0D]='_',
        [0x10]='F',[0x11]='G',[0x12]=0x011E,[0x13]='I',[0x14]='O',
        [0x15]='D',[0x16]='R',[0x17]='N',[0x18]='H',[0x19]='P',
        [0x1A]='Q',[0x1B]='W',
        [0x1E]='U',[0x1F]=0x0130,[0x20]='E',[0x21]='A',[0x22]=0x00DC,
        [0x23]='T',[0x24]='K',[0x25]='M',[0x26]='L',[0x27]='Y',
        [0x28]=0x015E,[0x2B]='X',
        [0x56]='>',
        [0x2C]='J',[0x2D]=0x00D6,[0x2E]='V',[0x2F]='C',[0x30]=0x00C7,
        [0x31]='Z',[0x32]='S',[0x33]='B',[0x34]=':',[0x35]=';',
    },
    /* altgr */ {
        COMMON_KEYS,
        [0x02]='>',[0x03]=0x00A3,[0x04]='#',[0x05]='$',[0x06]=0x00BD,
        [0x08]='{',[0x09]='[',[0x0A]=']',[0x0B]='}',[0x0C]='\\',[0x0D]='|',
        [0x12]=0x20AC,[0x1A]='@',
        [0x56]='|',
    },
    /* shift+altgr */ { COMMON_KEYS },
};

/* ================= German (QWERTZ) ================= */
static const keytable km_de = {
    /* plain */ {
        COMMON_KEYS,
        [0x29]=KEYMAP_DK(MARK_CIRCUMFLEX),
        [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',[0x07]='6',
        [0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',
        [0x0C]=0x00DF,[0x0D]=KEYMAP_DK(MARK_ACUTE),
        [0x10]='q',[0x11]='w',[0x12]='e',[0x13]='r',[0x14]='t',[0x15]='z',
        [0x16]='u',[0x17]='i',[0x18]='o',[0x19]='p',
        [0x1A]=0x00FC,[0x1B]='+',
        [0x1E]='a',[0x1F]='s',[0x20]='d',[0x21]='f',[0x22]='g',[0x23]='h',
        [0x24]='j',[0x25]='k',[0x26]='l',[0x27]=0x00F6,[0x28]=0x00E4,
        [0x2B]='#',
        [0x56]='<',
        [0x2C]='y',[0x2D]='x',[0x2E]='c',[0x2F]='v',[0x30]='b',[0x31]='n',
        [0x32]='m',[0x33]=',',[0x34]='.',[0x35]='-',
    },
    /* shift */ {
        COMMON_KEYS,
        [0x29]=0x00B0,
        [0x02]='!',[0x03]='"',[0x04]=0x00A7,[0x05]='$',[0x06]='%',[0x07]='&',
        [0x08]='/',[0x09]='(',[0x0A]=')',[0x0B]='=',
        [0x0C]='?',[0x0D]=KEYMAP_DK(MARK_GRAVE),
        [0x10]='Q',[0x11]='W',[0x12]='E',[0x13]='R',[0x14]='T',[0x15]='Z',
        [0x16]='U',[0x17]='I',[0x18]='O',[0x19]='P',
        [0x1A]=0x00DC,[0x1B]='*',
        [0x1E]='A',[0x1F]='S',[0x20]='D',[0x21]='F',[0x22]='G',[0x23]='H',
        [0x24]='J',[0x25]='K',[0x26]='L',[0x27]=0x00D6,[0x28]=0x00C4,
        [0x2B]='\'',
        [0x56]='>',
        [0x2C]='Y',[0x2D]='X',[0x2E]='C',[0x2F]='V',[0x30]='B',[0x31]='N',
        [0x32]='M',[0x33]=';',[0x34]=':',[0x35]='_',
    },
    /* altgr */ {
        COMMON_KEYS,
        [0x03]=0x00B2,[0x04]=0x00B3,
        [0x07]='{',[0x08]='[',[0x09]=']',[0x0A]='}',[0x0B]='\\',
        [0x0C]='\\',[0x10]='@',[0x12]=0x20AC,[0x1B]='~',
        [0x32]=0x00B5,[0x56]='|',
    },
    /* shift+altgr */ { COMMON_KEYS },
};

/* ================= French (AZERTY) ================= */
static const keytable km_fr = {
    /* plain */ {
        COMMON_KEYS,
        [0x29]=0x00B2,
        [0x02]='&',[0x03]=0x00E9,[0x04]='"',[0x05]='\'',[0x06]='(',
        [0x07]='-',[0x08]=0x00E8,[0x09]='_',[0x0A]=0x00E7,[0x0B]=0x00E0,
        [0x0C]=')',[0x0D]='=',
        [0x10]='a',[0x11]='z',[0x12]='e',[0x13]='r',[0x14]='t',[0x15]='y',
        [0x16]='u',[0x17]='i',[0x18]='o',[0x19]='p',
        [0x1A]=KEYMAP_DK(MARK_CIRCUMFLEX),[0x1B]='$',
        [0x1E]='q',[0x1F]='s',[0x20]='d',[0x21]='f',[0x22]='g',[0x23]='h',
        [0x24]='j',[0x25]='k',[0x26]='l',[0x27]='m',[0x28]=0x00F9,
        [0x2B]='*',
        [0x56]='<',
        [0x2C]='w',[0x2D]='x',[0x2E]='c',[0x2F]='v',[0x30]='b',[0x31]='n',
        [0x32]=',',[0x33]=';',[0x34]=':',[0x35]='!',
    },
    /* shift */ {
        COMMON_KEYS,
        [0x29]='~',
        [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',[0x07]='6',
        [0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',
        [0x0C]=0x00B0,[0x0D]='+',
        [0x10]='A',[0x11]='Z',[0x12]='E',[0x13]='R',[0x14]='T',[0x15]='Y',
        [0x16]='U',[0x17]='I',[0x18]='O',[0x19]='P',
        [0x1A]=KEYMAP_DK(MARK_DIAERESIS),[0x1B]=0x00A3,
        [0x1E]='Q',[0x1F]='S',[0x20]='D',[0x21]='F',[0x22]='G',[0x23]='H',
        [0x24]='J',[0x25]='K',[0x26]='L',[0x27]='M',[0x28]='%',
        [0x2B]=0x00B5,
        [0x56]='>',
        [0x2C]='W',[0x2D]='X',[0x2E]='C',[0x2F]='V',[0x30]='B',[0x31]='N',
        [0x32]='?',[0x33]='.',[0x34]='/',[0x35]=0x00A7,
    },
    /* altgr */ {
        COMMON_KEYS,
        [0x03]='~',[0x04]='#',[0x05]='{',[0x06]='[',[0x07]='|',
        [0x08]='`',[0x09]='\\',[0x0A]='^',[0x0B]='@',[0x0C]=']',[0x0D]='}',
        [0x12]=0x20AC,[0x1B]=0x00A4,
        [0x56]='|',
    },
    /* shift+altgr */ { COMMON_KEYS },
};

/* ================= Spanish ================= */
static const keytable km_es = {
    /* plain */ {
        COMMON_KEYS,
        [0x29]=0x00BA,
        [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',[0x06]='5',[0x07]='6',
        [0x08]='7',[0x09]='8',[0x0A]='9',[0x0B]='0',
        [0x0C]='\'',[0x0D]=0x00A1,
        [0x10]='q',[0x11]='w',[0x12]='e',[0x13]='r',[0x14]='t',[0x15]='y',
        [0x16]='u',[0x17]='i',[0x18]='o',[0x19]='p',
        [0x1A]=KEYMAP_DK(MARK_GRAVE),[0x1B]='+',
        [0x1E]='a',[0x1F]='s',[0x20]='d',[0x21]='f',[0x22]='g',[0x23]='h',
        [0x24]='j',[0x25]='k',[0x26]='l',[0x27]=0x00F1,
        [0x28]=KEYMAP_DK(MARK_ACUTE),[0x2B]=0x00E7,
        [0x56]='<',
        [0x2C]='z',[0x2D]='x',[0x2E]='c',[0x2F]='v',[0x30]='b',[0x31]='n',
        [0x32]='m',[0x33]=',',[0x34]='.',[0x35]='-',
    },
    /* shift */ {
        COMMON_KEYS,
        [0x29]=0x00AA,
        [0x02]='!',[0x03]='"',[0x04]=0x00B7,[0x05]='$',[0x06]='%',[0x07]='&',
        [0x08]='/',[0x09]='(',[0x0A]=')',[0x0B]='=',
        [0x0C]='?',[0x0D]=0x00BF,
        [0x10]='Q',[0x11]='W',[0x12]='E',[0x13]='R',[0x14]='T',[0x15]='Y',
        [0x16]='U',[0x17]='I',[0x18]='O',[0x19]='P',
        [0x1A]=KEYMAP_DK(MARK_CIRCUMFLEX),[0x1B]='*',
        [0x1E]='A',[0x1F]='S',[0x20]='D',[0x21]='F',[0x22]='G',[0x23]='H',
        [0x24]='J',[0x25]='K',[0x26]='L',[0x27]=0x00D1,
        [0x28]=KEYMAP_DK(MARK_DIAERESIS),[0x2B]=0x00C7,
        [0x56]='>',
        [0x2C]='Z',[0x2D]='X',[0x2E]='C',[0x2F]='V',[0x30]='B',[0x31]='N',
        [0x32]='M',[0x33]=';',[0x34]=':',[0x35]='_',
    },
    /* altgr */ {
        COMMON_KEYS,
        [0x29]='\\',[0x02]='|',[0x03]='@',[0x04]='#',[0x05]='~',
        [0x06]=0x20AC,[0x07]=0x00AC,
        [0x1A]='[',[0x1B]=']',[0x27]='{',[0x28]='}',
        [0x56]='|',
    },
    /* shift+altgr */ { COMMON_KEYS },
};

/* ---- the layout list ---- */

struct layout {
    const char *name;
    const char *description;
    const keytable *table;
};

static const struct layout g_layouts[] = {
    { "us",   "US English (QWERTY)",          &km_us  },
    { "tr",   "Turkish Q (QWERTY)",           &km_tr  },
    { "tr-f", "Turkish F",                    &km_trf },
    { "de",   "German (QWERTZ)",              &km_de  },
    { "fr",   "French (AZERTY)",              &km_fr  },
    { "es",   "Spanish",                      &km_es  },
};

#define LAYOUT_COUNT ((int)(sizeof(g_layouts) / sizeof(g_layouts[0])))

static const struct layout *g_current = &g_layouts[0];

int keymap_set(const char *name) {
    if (name == NULL) {
        return -1;
    }
    for (int i = 0; i < LAYOUT_COUNT; i++) {
        if (strcmp(g_layouts[i].name, name) == 0) {
            g_current = &g_layouts[i];
            return 0;
        }
    }
    return -1;
}

const char *keymap_name(void) {
    return g_current->name;
}

bool keymap_at(int index, const char **name, const char **description) {
    if (index < 0 || index >= LAYOUT_COUNT) {
        return false;
    }
    if (name != NULL) {
        *name = g_layouts[index].name;
    }
    if (description != NULL) {
        *description = g_layouts[index].description;
    }
    return true;
}

/* Is this codepoint a letter, for the purpose of Caps Lock?
 *
 * "Letter" here means the Latin ranges the layouts use: ASCII, the
 * Latin-1 supplement past the punctuation block, and Latin Extended-A
 * and B. Digits and punctuation are not letters, which is the whole
 * point - Caps Lock must not turn 1 into !. */
static bool is_letter(uint32_t cp) {
    if (cp >= 'a' && cp <= 'z') return true;
    if (cp >= 'A' && cp <= 'Z') return true;
    if (cp >= 0x00C0 && cp <= 0x00FF && cp != 0x00D7 && cp != 0x00F7) {
        return true; /* the multiplication and division signs are not */
    }
    if (cp >= 0x0100 && cp <= 0x024F) return true;
    return false;
}

uint32_t keymap_lookup(uint8_t scancode, bool shift, bool altgr, bool caps) {
    if (scancode >= CODES) {
        return 0;
    }
    const keytable *t = g_current->table;

    int level = (shift ? 1 : 0) | (altgr ? 2 : 0);
    uint32_t cp = (*t)[level][scancode];

    /* Caps Lock is Shift for letters and nothing for anything else.
     * It is applied by picking the OTHER level rather than by
     * upper-casing the result, because upper-casing cannot know that
     * on a Turkish layout the capital of 'i' is a dotted I and the
     * capital of a dotless i is a plain I. The table already knows. */
    if (caps && !altgr) {
        uint32_t plain = (*t)[0][scancode];
        if (is_letter(plain)) {
            cp = (*t)[shift ? 0 : 1][scancode];
        }
    }
    return cp;
}

/* ---- dead key composition ----
 *
 * Sorted by (mark, base) so the search can halve. Every entry here has
 * a glyph in font_latin.h: composing to a character the console cannot
 * draw would turn a correct keystroke into a blank cell, which is
 * worse than refusing to compose and showing both characters. */
struct compose_entry {
    uint16_t mark;
    uint16_t base;
    uint16_t result;
};

static const struct compose_entry g_compose[] = {
    /* U+0300 grave */
    { 0x0300, 'A', 0x00C0 }, { 0x0300, 'E', 0x00C8 },
    { 0x0300, 'I', 0x00CC }, { 0x0300, 'O', 0x00D2 },
    { 0x0300, 'U', 0x00D9 },
    { 0x0300, 'a', 0x00E0 }, { 0x0300, 'e', 0x00E8 },
    { 0x0300, 'i', 0x00EC }, { 0x0300, 'o', 0x00F2 },
    { 0x0300, 'u', 0x00F9 },
    /* U+0301 acute */
    { 0x0301, 'A', 0x00C1 }, { 0x0301, 'C', 0x0106 },
    { 0x0301, 'E', 0x00C9 }, { 0x0301, 'I', 0x00CD },
    { 0x0301, 'N', 0x0143 }, { 0x0301, 'O', 0x00D3 },
    { 0x0301, 'S', 0x015A }, { 0x0301, 'U', 0x00DA },
    { 0x0301, 'Y', 0x00DD }, { 0x0301, 'Z', 0x0179 },
    { 0x0301, 'a', 0x00E1 }, { 0x0301, 'c', 0x0107 },
    { 0x0301, 'e', 0x00E9 }, { 0x0301, 'i', 0x00ED },
    { 0x0301, 'n', 0x0144 }, { 0x0301, 'o', 0x00F3 },
    { 0x0301, 's', 0x015B }, { 0x0301, 'u', 0x00FA },
    { 0x0301, 'y', 0x00FD }, { 0x0301, 'z', 0x017A },
    /* U+0302 circumflex */
    { 0x0302, 'A', 0x00C2 }, { 0x0302, 'E', 0x00CA },
    { 0x0302, 'I', 0x00CE }, { 0x0302, 'O', 0x00D4 },
    { 0x0302, 'U', 0x00DB },
    { 0x0302, 'a', 0x00E2 }, { 0x0302, 'e', 0x00EA },
    { 0x0302, 'i', 0x00EE }, { 0x0302, 'o', 0x00F4 },
    { 0x0302, 'u', 0x00FB },
    /* U+0303 tilde */
    { 0x0303, 'A', 0x00C3 }, { 0x0303, 'N', 0x00D1 },
    { 0x0303, 'O', 0x00D5 },
    { 0x0303, 'a', 0x00E3 }, { 0x0303, 'n', 0x00F1 },
    { 0x0303, 'o', 0x00F5 },
    /* U+0308 diaeresis */
    { 0x0308, 'A', 0x00C4 }, { 0x0308, 'E', 0x00CB },
    { 0x0308, 'I', 0x00CF }, { 0x0308, 'O', 0x00D6 },
    { 0x0308, 'U', 0x00DC },
    { 0x0308, 'a', 0x00E4 }, { 0x0308, 'e', 0x00EB },
    { 0x0308, 'i', 0x00EF }, { 0x0308, 'o', 0x00F6 },
    { 0x0308, 'u', 0x00FC }, { 0x0308, 'y', 0x00FF },
    /* U+030A ring */
    { 0x030A, 'A', 0x00C5 }, { 0x030A, 'a', 0x00E5 },
    /* U+030C caron */
    { 0x030C, 'C', 0x010C }, { 0x030C, 'S', 0x0160 },
    { 0x030C, 'Z', 0x017D },
    { 0x030C, 'c', 0x010D }, { 0x030C, 's', 0x0161 },
    { 0x030C, 'z', 0x017E },
    /* U+0327 cedilla */
    { 0x0327, 'C', 0x00C7 }, { 0x0327, 'S', 0x015E },
    { 0x0327, 'c', 0x00E7 }, { 0x0327, 's', 0x015F },
};

#define COMPOSE_COUNT ((int)(sizeof(g_compose) / sizeof(g_compose[0])))

uint32_t keymap_compose(uint32_t mark, uint32_t base) {
    int lo = 0, hi = COMPOSE_COUNT - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        uint32_t key = ((uint32_t)g_compose[mid].mark << 16) |
                       g_compose[mid].base;
        uint32_t want = (mark << 16) | (base & 0xFFFF);
        if (key == want) {
            return g_compose[mid].result;
        }
        if (key < want) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return 0;
}

uint32_t keymap_spacing(uint32_t mark) {
    switch (mark) {
    case MARK_GRAVE:      return '`';
    case MARK_ACUTE:      return 0x00B4;
    case MARK_CIRCUMFLEX: return '^';
    case MARK_TILDE:      return '~';
    case MARK_DIAERESIS:  return 0x00A8;
    case MARK_RING:       return 0x00B0;
    case MARK_CARON:      return '^';  /* no spacing caron in this font */
    case MARK_CEDILLA:    return 0x00B8;
    default:              return 0;
    }
}

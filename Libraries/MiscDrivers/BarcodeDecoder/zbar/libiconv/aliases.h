/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -m 10 aliases.gperf  */
/* Computed positions: -k'1,3-11,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif


#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 45
#define MIN_HASH_VALUE 15
#define MAX_HASH_VALUE 921

struct alias { const char *name; unsigned int encoding_index; };

static const struct alias aliases[] =
  {
	{"ISO8859-1", 				ei_iso8859_1},
	{"UTF-8", 					ei_utf8},
	{"SJIS",	 				ei_sjis},
	{"BIG-5", 					ei_ces_big5},
#if 0
    {"L6", 						ei_iso8859_10},
    {"L1", 						ei_iso8859_1},
    {"CN", 						ei_iso646_cn},
    {"L4", 						ei_iso8859_4},
    {"L5", 						ei_iso8859_9},
    {"R8", 						ei_hp_roman8},
    {"L8", 						ei_iso8859_14},
    {"866", 					ei_cp866},
    {"L2", 						ei_iso8859_2},
    {"ISO-IR-6", 				ei_ascii},
    {"CP866", 					ei_cp866},
    {"C99", 					ei_c99},
    {"ISO-IR-166", 				ei_tis620},
    {"EUCCN", 					ei_euc_cn},
    {"LATIN6", 					ei_iso8859_10},
    {"CP154", 					ei_pt154},
    {"LATIN1", 					ei_iso8859_1},
    {"EUC-CN", 					ei_euc_cn},
    {"CYRILLIC", 				ei_iso8859_5},
    {"ISO646-CN", 				ei_iso646_cn},
    {"HZ", 						ei_hz},
    {"ISO-IR-14", 				ei_iso646_jp},
    {"CP1256", 					ei_cp1256},
    {"LATIN4", 					ei_iso8859_4},
    {"CP1251", 					ei_cp1251},
    {"ISO-IR-165", 				ei_isoir165},
    {"LATIN5", 					ei_iso8859_9},
    {"862", 					ei_cp862},
    {"ISO-IR-126", 				ei_iso8859_7},
    {"ISO-IR-144", 				ei_iso8859_5},
    {"CP819", 					ei_iso8859_1},
    {"LATIN8", 					ei_iso8859_14},
    {"CP1254", 					ei_cp1254},
    {"ISO-IR-58", 				ei_gb2312},
    {"CP949", 					ei_cp949},
    {"CP1255", 					ei_cp1255},
    {"CP862", 					ei_cp862},
    {"ISO-IR-148", 				ei_iso8859_9},
    {"L3", 						ei_iso8859_3},
    {"LATIN-9", 				ei_iso8859_15},
    {"CHINESE", 				ei_gb2312},
    {"ISO-IR-149", 				ei_ksc5601},
    {"ISO-IR-159", 				ei_jisx0212},
    {"ISO-IR-226", 				ei_iso8859_16},
    {"CP1258", 					ei_cp1258},
    {"PT154", 					ei_pt154},
    {"LATIN2", 					ei_iso8859_2},
    {"ISO8859-6", 				ei_iso8859_6},
    {"ISO-IR-199", 				ei_iso8859_14},
    {"ISO-CELTIC", 				ei_iso8859_14},
    {"ISO-8859-6", 				ei_iso8859_6},
    {"ISO_8859-6", 				ei_iso8859_6},
    {"ISO8859-16", 				ei_iso8859_16},
    {"ISO-8859-1", 				ei_iso8859_1},
    {"ISO_8859-1", 				ei_iso8859_1},
    {"ISO8859-11", 				ei_iso8859_11},
    {"ISO-8859-16", 			ei_iso8859_16},
    {"ISO_8859-16", 			ei_iso8859_16},
    {"CP1252", 					ei_cp1252},
    {"ISO-8859-11", 			ei_iso8859_11},
    {"ISO_8859-11", 			ei_iso8859_11},
    {"ISO8859-4", 				ei_iso8859_4},
    {"CP1361", 					ei_johab},
    {"ISO_8859-16:2001", 		ei_iso8859_16},
    {"CP1131", 					ei_cp1131},
    {"ISO8859-5", 				ei_iso8859_5},
    {"ISO-8859-4", 				ei_iso8859_4},
    {"ISO_8859-4", 				ei_iso8859_4},
    {"ISO8859-14", 				ei_iso8859_14},
    {"CP936", 					ei_cp936},
    {"ISO-8859-5", 				ei_iso8859_5},
    {"ISO_8859-5", 				ei_iso8859_5},
    {"ISO8859-15", 				ei_iso8859_15},
    {"MAC", 					ei_mac_roman},
    {"ISO-8859-14", 			ei_iso8859_14},
    {"ISO_8859-14", 			ei_iso8859_14},
    {"ISO8859-8", 				ei_iso8859_8},
    {"ISO-IR-101", 				ei_iso8859_2},
    {"ISO-8859-15", 			ei_iso8859_15},
    {"ISO_8859-15", 			ei_iso8859_15},
    {"ISO8859-9", 				ei_iso8859_9},
    {"ISO-8859-8", 				ei_iso8859_8},
    {"ISO_8859-8", 				ei_iso8859_8},
    {"ISO-8859-9", 				ei_iso8859_9},
    {"ISO_8859-9", 				ei_iso8859_9},
    {"ISO_8859-14:1998", 		ei_iso8859_14},
    {"ISO_8859-15:1998", 		ei_iso8859_15},
    {"RK1048", 					ei_rk1048},
    {"ELOT_928",	 			ei_iso8859_7},
    {"ISO8859-2", 				ei_iso8859_2},
    {"IBM866", 					ei_cp866},
    {"L7", 						ei_iso8859_13},
    {"US", 						ei_ascii},
    {"ISO-8859-2", 				ei_iso8859_2},
    {"ISO_8859-2", 				ei_iso8859_2},
    {"X0212", 					ei_jisx0212},
    {"CHAR", 					ei_local_char},
    {"PTCP154", 				ei_pt154},
    {"ISO-IR-109", 				ei_iso8859_3},
    {"ISO-IR-138", 				ei_iso8859_8},
    {"UHC", 					ei_cp949},
    {"ASCII", 					ei_ascii},
    {"MS-CYRL", 				ei_cp1251},
    {"L10", 					ei_iso8859_16},
    {"VISCII", 					ei_viscii},
    {"CP932", 					ei_cp932},
    {"BIG5", 					ei_ces_big5},
    {"TCVN", 					ei_tcvn},
    {"CP50221", 				ei_iso2022_jpms},
    {"MS-EE", 					ei_cp1250},
    {"850", 					ei_cp850},
    {"UCS-4", 					ei_ucs4},
    {"CSASCII", 				ei_ascii},
    {"IBM819", 					ei_iso8859_1},
    {"CSVISCII", 				ei_viscii},
    {"GB2312", 					ei_euc_cn},
    {"X0201", 					ei_jisx0201},
    {"CSBIG5", 					ei_ces_big5},
    {"CN-BIG5", 				ei_ces_big5},
    {"KOI8-R", 					ei_koi8_r},
    {"EUCKR", 					ei_euc_kr},
    {"IBM862", 					ei_cp862},
    {"CSPTCP154", 				ei_pt154},
    {"CSUCS4", 					ei_ucs4},
    {"ISO646-US", 				ei_ascii},
    {"CP874", 					ei_cp874},
    {"EUC-KR", 					ei_euc_kr},
    {"VISCII1.1-1", 			ei_viscii},
    {"ISO_646.IRV:1991", 		ei_ascii},
    {"MACCYRILLIC", 			ei_mac_cyrillic},
    {"CSKOI8R", 				ei_koi8_r},
    {"CP850", 					ei_cp850},
    {"ISO-IR-110", 				ei_iso8859_4},
    {"CP950", 					ei_cp950},
    {"ISO-2022-CN", 			ei_iso2022_cn},
    {"CN-GB-ISOIR165", 			ei_isoir165},
    {"CYRILLIC-ASIAN", 			ei_pt154},
    {"UCS-2", 					ei_ucs2},
    {"LATIN10", 				ei_iso8859_16},
    {"X0208", 					ei_jisx0208},
    {"LATIN3", 					ei_iso8859_3},
    {"CSISO2022CN", 			ei_iso2022_cn},
    {"ISO_8859-10:1992", 		ei_iso8859_10},
    {"UCS-4LE", 				ei_ucs4le},
    {"CP1250", 					ei_cp1250},
    {"CSISOLATIN6", 			ei_iso8859_10},
    {"ISO-IR-179", 				ei_iso8859_13},
    {"MS936", 					ei_cp936},
    {"CSISOLATIN1", 			ei_iso8859_1},
    {"TIS620", 					ei_tis620},
    {"CP1253", 					ei_cp1253},
    {"ISO-2022-CN-EXT", 		ei_iso2022_cn_ext},
    {"TIS-620", 				ei_tis620},
    {"UCS-2LE", 				ei_ucs2le},
    {"KOI8-T", 					ei_koi8_t},
    {"CSISOLATINCYRILLIC", 		ei_iso8859_5},
    {"CSISOLATIN4", 			ei_iso8859_4},
    {"CSISOLATIN5", 			ei_iso8859_9},
    {"UNICODE-1-1", 			ei_ucs2be},
    {"GREEK8", 					ei_iso8859_7},
    {"UCS-4-INTERNAL", 			ei_ucs4internal},
    {"CSUNICODE11", 			ei_ucs2be},
    {"CSUNICODE", 				ei_ucs2},
    {"TIS620.2529-1", 			ei_tis620},
    {"NEXTSTEP", 				ei_nextstep},
    {"GEORGIAN-PS", 			ei_georgian_ps},
    {"ISO8859-10", 				ei_iso8859_10},
    {"ISO8859-3", 				ei_iso8859_3},
    {"UCS-2-INTERNAL", 			ei_ucs2internal},
    {"ISO-8859-10", 			ei_iso8859_10},
    {"ISO_8859-10", 			ei_iso8859_10},
    {"ISO-8859-3", 				ei_iso8859_3},
    {"ISO_8859-3", 				ei_iso8859_3},
    {"ISO8859-13", 				ei_iso8859_13},
    {"CSISOLATIN2", 			ei_iso8859_2},
    {"GB_1988-80", 				ei_iso646_cn},
    {"ISO-8859-13", 			ei_iso8859_13},
    {"ISO_8859-13", 			ei_iso8859_13},
    {"CP1133", 					ei_cp1133},
    {"MS-ANSI", 				ei_cp1252},
    {"ISO-IR-100", 				ei_iso8859_1},
    {"KSC_5601", 				ei_ksc5601},
    {"MACINTOSH", 				ei_mac_roman},
    {"CN-GB", 					ei_euc_cn},
    {"CSIBM866", 				ei_cp866},
    {"ISO-10646-UCS-4", 		ei_ucs4},
    {"ISO_8859-4:1988", 		ei_iso8859_4},
    {"ISO_8859-5:1988", 		ei_iso8859_5},
    {"UTF-16", 					ei_utf16},
    {"KZ-1048", 				ei_rk1048},
    {"ROMAN8", 					ei_hp_roman8},
    {"ISO_8859-8:1988", 		ei_iso8859_8},
    {"UNICODELITTLE", 			ei_ucs2le},
    {"ISO_8859-9:1989", 		ei_iso8859_9},
    {"JP", 						ei_iso646_jp},
    {"ISO-10646-UCS-2", 		ei_ucs2},
    {"MULELAO-1", 				ei_mulelao},
    {"CSKZ1048", 				ei_rk1048},
    {"ISO-IR-203", 				ei_iso8859_15},
    {"IBM850", 					ei_cp850},
    {"TIS620-0", 				ei_tis620},
    {"ECMA-114", 				ei_iso8859_6},
    {"KS_C_5601-1989", 			ei_ksc5601},
    {"GBK", 					ei_ces_gbk},
    {"ARMSCII-8", 				ei_armscii_8},
    {"US-ASCII", 				ei_ascii},
    {"ECMA-118", 				ei_iso8859_7},
    {"CSGB2312", 				ei_euc_cn},
    {"LATIN7", 					ei_iso8859_13},
    {"KOREAN", 					ei_ksc5601},
    {"ISO-IR-57", 				ei_iso646_cn},
    {"MS-HEBR", 				ei_cp1255},
    {"CSISO14JISC6220RO", 		ei_iso646_jp},
    {"ISO-IR-87", 				ei_jisx0208},
    {"ISO-IR-157", 				ei_iso8859_10},
    {"UTF-16LE", 				ei_utf16le},
    {"UCS-4BE", 				ei_ucs4be},
    {"CP1257", 					ei_cp1257},
    {"ANSI_X3.4-1986", 			ei_ascii},
    {"CSEUCKR", 				ei_euc_kr},
    {"ISO-2022-KR", 			ei_iso2022_kr},
    {"GB_2312-80", 				ei_gb2312},
    {"ISO-IR-127", 				ei_iso8859_6},
    {"MACTHAI", 				ei_mac_thai},
    {"EUCTW", 					ei_euc_tw},
    {"EXTENDED_UNIX_CODE_PACKED_FORMAT_FOR_JAPANESE", ei_euc_jp},
    {"CSISO2022KR", 			ei_iso2022_kr},
    {"UCS-2BE", 				ei_ucs2be},
    {"EUC-TW", 					ei_euc_tw},
    {"ANSI_X3.4-1968", 			ei_ascii},
    {"GREEK", 					ei_iso8859_7},
    {"MACICELAND", 				ei_mac_iceland},
    {"GB18030", 				ei_gb18030},
    {"ISO_8859-3:1988", 		ei_iso8859_3},
    {"CSISOLATINARABIC", 		ei_iso8859_6},
    {"GEORGIAN-ACADEMY", 		ei_georgian_academy},
    {"HP-ROMAN8", 				ei_hp_roman8},
    {"TIS620.2533-1", 			ei_tis620},
    {"UNICODEBIG", 				ei_ucs2be},
    {"TCVN5712-1", 				ei_tcvn},
    {"ISO8859-7", 				ei_iso8859_7},
    {"CSISO57GB1988", 			ei_iso646_cn},
    {"MACCENTRALEUROPE", 		ei_mac_centraleurope},
    {"CSISOLATINGREEK", 		ei_iso8859_7},
    {"CSISOLATIN3", 			ei_iso8859_3},
    {"ISO-8859-7", 				ei_iso8859_7},
    {"ISO_8859-7", 				ei_iso8859_7},
    {"UNICODE-1-1-UTF-7", 		ei_utf7},
    {"CP367", 					ei_ascii},
    {"HZ-GB-2312",	 			ei_hz},
    {"EUCJP", 					ei_euc_jp},
    {"CSUNICODE11UTF7", 		ei_utf7},
    {"MACROMAN", 				ei_mac_roman},
    {"TCVN-5712", 				ei_tcvn},
    {"EUC-JP", 					ei_euc_jp},
    {"ISO646-JP", 				ei_iso646_jp},
    {"BIG5HKSCS", 				ei_big5hkscs2008},
    {"CSISO58GB231280", 		ei_gb2312},
    {"WINDOWS-1256", 			ei_cp1256},
    {"WINDOWS-1251", 			ei_cp1251},
    {"BIG5-HKSCS", 				ei_big5hkscs2008},
    {"WCHAR_T", 				ei_local_wchar_t},
    {"ISO_8859-6:1987", 		ei_iso8859_6},
    {"ISO_8859-1:1987", 		ei_iso8859_1},
    {"WINDOWS-1254", 			ei_cp1254},
    {"WINDOWS-1255", 			ei_cp1255},
    {"WINDOWS-1258", 			ei_cp1258},
    {"UTF-32", 					ei_utf32},
    {"CSISOLATINHEBREW", 		ei_iso8859_8},
    {"CSHPROMAN8", 				ei_hp_roman8},
    {"CSISO159JISX02121990", 	ei_jisx0212},
    {"WINDOWS-1252", 			ei_cp1252},
    {"STRK1048-2002", 			ei_rk1048},
    {"ISO_8859-2:1987", 		ei_iso8859_2},
    {"ARABIC", 					ei_iso8859_6},
    {"CSMACINTOSH", 			ei_mac_roman},
    {"KS_C_5601-1987", 			ei_ksc5601},
    {"JIS_C6220-1969-RO", 		ei_iso646_jp},
    {"JIS_C6226-1983", 			ei_jisx0208},
    {"UTF-32LE", 				ei_utf32le},
    {"TIS620.2533-0", 			ei_tis620},
    {"IBM-CP1133", 				ei_cp1133},
    {"SHIFT-JIS",	 			ei_sjis},
    {"SHIFT_JIS", 				ei_sjis},
    {"CSPC862LATINHEBREW", 		ei_cp862},
    {"BIGFIVE", 				ei_ces_big5},
    {"CSKSC56011987", 			ei_ksc5601},
    {"MACCROATIAN", 			ei_mac_croatian},
    {"BIG-FIVE", 				ei_ces_big5},
    {"WINDOWS-936", 			ei_cp936},
    {"UTF-16BE", 				ei_utf16be},
    {"KOI8-U", 					ei_koi8_u},
    {"KOI8-RU", 				ei_koi8_ru},
    {"WINDOWS-1250", 			ei_cp1250},
    {"MS-GREEK", 				ei_cp1253},
    {"WINDOWS-1253", 			ei_cp1253},
    {"MACGREEK", 				ei_mac_greek},
    {"CSPC850MULTILINGUAL", 	ei_cp850},
    {"ISO_8859-7:2003", 		ei_iso8859_7},
    {"JIS0208", 				ei_jisx0208},
    {"IBM367", 					ei_ascii},
    {"ISO-2022-JP-1", 			ei_iso2022_jp1},
    {"ISO-2022-JP-MS", 			ei_iso2022_jpms},
    {"JIS_X0212", 				ei_jisx0212},
    {"CSEUCTW", 				ei_euc_tw},
    {"CSSHIFTJIS", 				ei_sjis},
    {"HEBREW", 					ei_iso8859_8},
    {"ISO-2022-JP-2", 			ei_iso2022_jp2},
    {"ASMO-708", 				ei_iso8859_6},
    {"CSISO2022JP2", 			ei_iso2022_jp2},
    {"JIS_X0201", 				ei_jisx0201},
    {"WINDOWS-874", 			ei_cp874},
    {"WINDOWS-1257", 			ei_cp1257},
    {"ISO-2022-JP", 			ei_iso2022_jp},
    {"ISO_8859-7:1987", 		ei_iso8859_7},
    {"CSISO87JISX0208", 		ei_jisx0208},
    {"CSISO2022JP", 			ei_iso2022_jp},
    {"MACARABIC", 				ei_mac_arabic},
    {"JIS_X0208", 				ei_jisx0208},
    {"JISX0201-1976", 			ei_jisx0201},
    {"UTF-7", 					ei_utf7},
    {"UTF-32BE", 				ei_utf32be},
    {"BIG5-HKSCS:2001", 		ei_big5hkscs2001},
    {"JIS_X0212-1990", 			ei_jisx0212},
    {"JIS_X0212.1990-0", 		ei_jisx0212},
    {"BIG5-HKSCS:2004", 		ei_big5hkscs2004},
    {"MACTURKISH", 				ei_mac_turkish},
    {"BIG5-HKSCS:2008", 		ei_big5hkscs2008},
    {"BIG5-HKSCS:1999", 		ei_big5hkscs1999},
    {"MS-TURK", 				ei_cp1254},
    {"UCS-4-SWAPPED", 			ei_ucs4swapped},
    {"MACUKRAINE", 				ei_mac_ukraine},
    {"UCS-2-SWAPPED", 			ei_ucs2swapped},
    {"TCVN5712-1:1993", 		ei_tcvn},
    {"JIS_X0208-1990", 			ei_jisx0208},
    {"JIS_X0208-1983", 			ei_jisx0208},
    {"MACHEBREW", 				ei_mac_hebrew},
    {"MS-ARAB", 				ei_cp1256},
    {"JAVA", 					ei_java},
    {"MACROMANIA", 				ei_mac_romania},
    {"MS_KANJI", 				ei_sjis},
    {"WINBALTRIM", 				ei_cp1257},
    {"CSEUCPKDFMTJAPANESE", 	ei_euc_jp},
    {"JOHAB", 					ei_johab},
    {"CSHALFWIDTHKATAKANA", 	ei_jisx0201}
#endif
  };

const struct alias *aliases_lookup (register const char *str, register size_t len)
{
	int i;
	int number_of_item;
	register const char *s;

	number_of_item = sizeof(aliases) / sizeof(aliases[0]);

	if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
	{
		for (i = 0; i < number_of_item; i++) {
			s = aliases[i].name;
			if (*str == *s && !strcmp (str + 1, s + 1))
			  return &aliases[i];
		}
	}

	return 0;
}

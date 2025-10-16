/* Extracted from interp.c:53096 (function fileTimesInUTC). */

int
fileTimesInUTC(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((GIV(imageHeaderFlags) & 0x200) != 0);
}
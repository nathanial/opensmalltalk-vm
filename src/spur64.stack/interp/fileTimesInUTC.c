/* Extracted from interp.c:53096 (function fileTimesInUTC). */

/*	Answer if file times, including those from the FilePlugin and
	FileAttributesPlugin, should be answered in UTC seconds from the epoch
	rather than local seconds. */

	/* StackInterpreter>>#fileTimesInUTC */

int
fileTimesInUTC(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return ((GIV(imageHeaderFlags) & 0x200) != 0);
}
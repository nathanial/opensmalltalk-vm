/* Extracted from interp.c:54412 (function getThisSessionID). */

sqInt
getThisSessionID(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(globalSessionID);
}
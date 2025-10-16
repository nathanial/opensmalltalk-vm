/* Extracted from interp.c:54367 (function getNextWakeupUsecs). */

usqLong
getNextWakeupUsecs(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(nextWakeupUsecs);
}
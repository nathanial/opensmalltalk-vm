/* Extracted from interp.c:54367 (function getNextWakeupUsecs). */

	/* StackInterpreter>>#getNextWakeupUsecs */

usqLong
getNextWakeupUsecs(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return nextWakeupUsecs;
}
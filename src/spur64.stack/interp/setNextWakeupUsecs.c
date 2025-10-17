/* Extracted from interp.c:63025 (function setNextWakeupUsecs). */

	/* StackInterpreter>>#setNextWakeupUsecs: */

void
setNextWakeupUsecs(usqLong value)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	GIV(nextWakeupUsecs) = value;
}
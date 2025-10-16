/* Extracted from interp.c:63017 (function setInterruptPending). */

void
setInterruptPending(sqInt value)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	GIV(interruptPending) = value;
	forceInterruptCheck();
}
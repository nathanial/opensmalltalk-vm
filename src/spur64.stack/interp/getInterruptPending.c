/* Extracted from interp.c:54350 (function getInterruptPending). */

	/* StackInterpreter>>#getInterruptPending */

sqInt
getInterruptPending(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(interruptPending);
}
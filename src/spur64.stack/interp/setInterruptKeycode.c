/* Extracted from interp.c:63002 (function setInterruptKeycode). */

void
setInterruptKeycode(sqInt value)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	GIV(interruptKeycode) = value;
}
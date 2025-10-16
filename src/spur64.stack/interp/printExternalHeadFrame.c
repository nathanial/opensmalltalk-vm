/* Extracted from interp.c:59672 (function printExternalHeadFrame). */

void
printExternalHeadFrame(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	printFrameWithSP(GIV(framePointer), GIV(stackPointer));
}
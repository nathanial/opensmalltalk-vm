/* Extracted from interp.c:59672 (function printExternalHeadFrame). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printExternalHeadFrame */

void
printExternalHeadFrame(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	printFrameWithSP(framePointer, stackPointer);
}
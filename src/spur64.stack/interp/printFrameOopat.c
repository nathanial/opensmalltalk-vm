/* Extracted from interp.c:59733 (function printFrameOopat). */

	/* StackInterpreter>>#printFrameOop:at: */

static NoDbgRegParms void
printFrameOopat(char *name, char *address)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt it;

	it = longAt(address);

	/* begin printFrameAddress: */
	fprintf(transcript,
			"%16p:",
			((void *)address));
	fprintf(transcript,
			"%12s: %16p\t=" /* pst: */,
			name,
			((void *)it));
	printOopShortInner(it);
	cr();
}
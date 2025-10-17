/* Extracted from interp.c:59700 (function printFrameFlagsForFP). */

	/* StackInterpreter>>#printFrameFlagsForFP: */

static NoDbgRegParms void
printFrameFlagsForFP(char *theFP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *address;
    sqInt it;

	address = theFP + FoxFrameFlags;
	it = longAt(address);

	/* begin printFrameAddress: */
	fprintf(transcript,
			"%16p:",
			((void *)address));
	fprintf(transcript,
			"       flags: %p",
			((void *)it));
	if (it) {
		fprintf(transcript,
				"=%d",
				((int) it));
	}
	fprintf(transcript,
			"  numArgs: %d %sContext %sBlock\n",
			((int) (byteAt((theFP + FoxFrameFlags) + 1))),
			(byteAt((theFP + FoxFrameFlags) + 2)
			? "is"
			: "no"),
			(byteAt((theFP + FoxFrameFlags) + 3)
			? "is"
			: "not"));
}
/* Extracted from interp.c:59700 (function printFrameFlagsForFP). */

static NoDbgRegParms void
printFrameFlagsForFP(char *theFP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *address;
    sqInt it;

	address = theFP + FoxFrameFlags;
	it = longAt(address);

	/* begin printFrameAddress: */
	fprintf(GIV(transcript),
			"%16p:",
			((void *)address));
	fprintf(GIV(transcript),
			"       flags: %p",
			((void *)it));
	if (it) {
		fprintf(GIV(transcript),
				"=%d",
				((int) it));
	}
	fprintf(GIV(transcript),
			"  numArgs: %d %sContext %sBlock\n",
			((int) (byteAt((theFP + FoxFrameFlags) + 1))),
			(byteAt((theFP + FoxFrameFlags) + 2)
			? "is"
			: "no"),
			(byteAt((theFP + FoxFrameFlags) + 3)
			? "is"
			: "not"));
}
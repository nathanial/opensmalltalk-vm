/* Extracted from interp.c:59815 (function printFrameThingatextraString). */

	/* StackInterpreter>>#printFrameThing:at:extraString: */

static NoDbgRegParms void
printFrameThingatextraString(char *name, char *address, char *extraStringOrNil)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt it;

	it = longAt(address);

	/* begin printFrameAddress: */
	fprintf(GIV(transcript),
			"%16p:",
			((void *)address));
	fprintf(GIV(transcript),
			"%12s: %16p" /* pst: */,
			name,
			((void *)it));
	framePrintDescription(it);
	if (extraStringOrNil) {
		fprintf(GIV(transcript),
				"%s",
				extraStringOrNil);
	}
	cr();
}
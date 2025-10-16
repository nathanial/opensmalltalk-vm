/* Extracted from interp.c:59784 (function printFrameThingandFrameat). */

static NoDbgRegParms void
printFrameThingandFrameat(char *name, char *theFP, char *address)
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
	if (it) {
		if (it == GIV(nilObj)) {
			fprintf(GIV(transcript), "=nil");
		}
		else {
			fprintf(GIV(transcript),
					"=%" PRIdSQINT "",
					it);
		}
	}
	fprintf(GIV(transcript),
			" frame: %p\n",
			theFP);
}
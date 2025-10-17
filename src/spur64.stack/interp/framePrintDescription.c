/* Extracted from interp.c:54022 (function framePrintDescription). */

	/* StackInterpreter>>#framePrintDescription: */

static NoDbgRegParms void
framePrintDescription(sqInt it)
{
	if (it) {
		if (it == nilObj) {
			fprintf(transcript, "=nil");
		}
		else {
			fprintf(transcript,
					"=%" PRIdSQINT "",
					it);
		}
	}
}
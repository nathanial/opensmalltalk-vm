/* Extracted from interp.c:54022 (function framePrintDescription). */

static NoDbgRegParms void
framePrintDescription(sqInt it)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
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
}
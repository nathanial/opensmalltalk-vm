/* Extracted from interp.c:60343 (function printNum). */

static NoDbgRegParms void
printNum(sqInt n)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	fprintf(GIV(transcript),
			"%" PRIdSQINT "",
			((sqInt)n));
}
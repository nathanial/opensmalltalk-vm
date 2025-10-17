/* Extracted from interp.c:60343 (function printNum). */

	/* StackInterpreter>>#printNum: */

static NoDbgRegParms void
printNum(sqInt n)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	fprintf(transcript,
			"%" PRIdSQINT "",
			((sqInt)n));
}
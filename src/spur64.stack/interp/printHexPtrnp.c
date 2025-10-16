/* Extracted from interp.c:59997 (function printHexPtrnp). */

static NoDbgRegParms sqInt
printHexPtrnp(void *p)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return fprintf(GIV(transcript),
			"%p",
			p);
}
/* Extracted from interp.c:60013 (function printHex). */

void
printHex(usqInt n)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	fprintf(GIV(transcript),
			"%16p" /* pst: */,
			((void *)n));
}
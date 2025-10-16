/* Extracted from interp.c:42631 (function printBogons). */

void
printBogons(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin printTheBogons: */
	print("bogon ");

	/* printHexnp: */
	fprintf(GIV(transcript),
			"%p",
			((void *)GIV(bogon)));
	cr();
	print("anomaly ");

	/* printHexnp: */
	fprintf(GIV(transcript),
			"%p",
			((void *)GIV(anomaly)));
	cr();
}
/* Extracted from interp.c:42631 (function printBogons). */

/*	useful for VM debugging */

	/* SpurMemoryManager>>#printBogons */

void
printBogons(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin printTheBogons: */
	print("bogon ");

	/* printHexnp: */
	fprintf(transcript,
			"%p",
			((void *)bogon));
	cr();
	print("anomaly ");

	/* printHexnp: */
	fprintf(transcript,
			"%p",
			((void *)anomaly));
	cr();
}
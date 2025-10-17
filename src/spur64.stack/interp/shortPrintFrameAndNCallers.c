/* Extracted from interp.c:63276 (function shortPrintFrameAndNCallers). */

/*	useful for VM debugging */

	/* StackInterpreter>>#shortPrintFrame:AndNCallers: */

void
shortPrintFrameAndNCallers(char *theFP, sqInt n)
{
	if ((n != 0)
	 && (/* couldBeFramePointer: */
		(((((usqInt)theFP)) & (BytesPerWord - 1)) == 0)
	 && ((((((usqInt)theFP)) >= (((usqInt)stackMemory))) && ((((usqInt)theFP)) <= (((usqInt)pages))))))) {
		shortPrintFrame(theFP);
		shortPrintFrameAndNCallers(((char *)(longAt(theFP + FoxSavedFP))), n - 1);
	}
}
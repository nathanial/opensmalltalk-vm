/* Extracted from interp.c:63276 (function shortPrintFrameAndNCallers). */

void
shortPrintFrameAndNCallers(char *theFP, sqInt n)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if ((n != 0)
	 && (/* couldBeFramePointer: */
		(((((usqInt)theFP)) & (BytesPerWord - 1)) == 0)
	 && ((((((usqInt)theFP)) >= (((usqInt)GIV(stackMemory)))) && ((((usqInt)theFP)) <= (((usqInt)GIV(pages)))))))) {
		shortPrintFrame(theFP);
		shortPrintFrameAndNCallers(((char *)(longAt(theFP + FoxSavedFP))), n - 1);
	}
}
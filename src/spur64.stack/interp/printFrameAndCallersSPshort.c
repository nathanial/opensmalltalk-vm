/* Extracted from interp.c:59679 (function printFrameAndCallersSPshort). */

static NoDbgRegParms sqInt
printFrameAndCallersSPshort(char *theFP, char *theSP, sqInt printShort)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (!(/* couldBeFramePointer: */
			(((((usqInt)theFP)) & (BytesPerWord - 1)) == 0)
		 && ((((((usqInt)theFP)) >= (((usqInt)GIV(stackMemory)))) && ((((usqInt)theFP)) <= (((usqInt)GIV(pages)))))))) {
		return null;
	}
	if (longAt(theFP + FoxSavedFP)) {
		printFrameAndCallersSPshort(((char *)(longAt(theFP + FoxSavedFP))), frameCallerSP(theFP), printShort);
	}
	if (printShort) {
		shortPrintFrame(theFP);
		return null;
	}
	cr();
	printFrameWithSP(theFP, theSP);
	return 0;
}
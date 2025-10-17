/* Extracted from interp.c:63190 (function shortPrintFrameAndCallers). */

	/* StackInterpreter>>#shortPrintFrameAndCallers: */

static NoDbgRegParms sqInt
shortPrintFrameAndCallers(char *theFP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (!(/* couldBeFramePointer: */
			(((((usqInt)theFP)) & (BytesPerWord - 1)) == 0)
		 && ((((((usqInt)theFP)) >= (((usqInt)GIV(stackMemory)))) && ((((usqInt)theFP)) <= (((usqInt)GIV(pages)))))))) {
		return null;
	}
	shortPrintFrame(theFP);
	if (longAt(theFP + FoxSavedFP)) {
		shortPrintFrameAndCallers(((char *)(longAt(theFP + FoxSavedFP))));
	}
	return 0;
}
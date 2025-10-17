/* Extracted from interp.c:63321 (function shortReversePrintFrameAndCallers). */

	/* StackInterpreter>>#shortReversePrintFrameAndCallers: */

static NoDbgRegParms sqInt
shortReversePrintFrameAndCallers(char *aFramePointer)
{
    sqInt caller;
    char *callerFP;
    char *theFP;

	callerFP = ((char *) 0);
	if (!aFramePointer) {
		return nilObj;
	}
	theFP = aFramePointer;
	while (1) {
		shortPrintFrame(theFP);
		callerFP = ((char *)(longAt(theFP + FoxSavedFP)));
		if (!(callerFP != 0)) break;
		theFP = callerFP;
	}

	/* begin frameCallerContext: */
	assert(isBaseFrame(theFP));
	caller = longAt(theFP + FoxCallerContext);
	if ((!((longAt((void *)(caller))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		caller = followForwarded(caller);
	}
	return caller;
}
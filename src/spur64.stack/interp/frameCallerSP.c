/* Extracted from interp.c:53947 (function frameCallerSP). */

static NoDbgRegParms char *
frameCallerSP(char *theFP)
{
	assert(!(isBaseFrame(theFP)));
	return (theFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord;
}
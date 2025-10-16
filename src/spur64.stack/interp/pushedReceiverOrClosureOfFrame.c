/* Extracted from interp.c:61204 (function pushedReceiverOrClosureOfFrame). */

static NoDbgRegParms sqInt
pushedReceiverOrClosureOfFrame(char *theFP)
{
	return longAt(theFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord()))))));
}
/* Extracted from interp.c:54052 (function frameStackedReceiverOffset). */

static NoDbgRegParms sqInt
frameStackedReceiverOffset(char *theFP)
{
	return (FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord()))));
}
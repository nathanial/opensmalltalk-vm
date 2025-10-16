/* Extracted from interp.c:65135 (function temporaryin). */

static NoDbgRegParms sqInt
temporaryin(sqInt offset, char *theFP)
{
    usqInt frameNumArgs;

	return (offset < ((frameNumArgs = byteAt((theFP + FoxFrameFlags) + 1)))
			? longAt((theFP + FoxCallerSavedIP) + ((frameNumArgs - offset) * BytesPerWord))
			: longAt(((theFP + FoxReceiver) - BytesPerWord) + ((frameNumArgs - offset) * BytesPerWord)));
}
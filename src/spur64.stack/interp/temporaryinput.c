/* Extracted from interp.c:65149 (function temporaryinput). */

static NoDbgRegParms sqInt
temporaryinput(sqInt offset, char *theFP, sqInt valueOop)
{
    usqInt frameNumArgs;

	return (offset < ((frameNumArgs = byteAt((theFP + FoxFrameFlags) + 1)))
			? longAtput((theFP + FoxCallerSavedIP) + ((frameNumArgs - offset) * BytesPerWord),valueOop)
			: longAtput(((theFP + FoxReceiver) - BytesPerWord) + ((frameNumArgs - offset) * BytesPerWord),valueOop));
}
/* Extracted from interp.c:53975 (function frameIsBlockActivation). */

static NoDbgRegParms int
frameIsBlockActivation(char *theFP)
{
	return (byteAt((theFP + FoxFrameFlags) + 3)) != 0;
}
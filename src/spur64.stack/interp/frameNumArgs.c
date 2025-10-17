/* Extracted from interp.c:54002 (function frameNumArgs). */

/*	See encodeFrameFieldHasContext:numArgs: */

	/* StackInterpreter>>#frameNumArgs: */

static NoDbgRegParms usqInt
frameNumArgs(char *theFP)
{
	return byteAt((theFP + FoxFrameFlags) + 1);
}
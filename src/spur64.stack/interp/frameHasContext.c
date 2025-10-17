/* Extracted from interp.c:53965 (function frameHasContext). */

/*	See encodeFrameFieldHasContext:numArgs: */

/* StackInterpreter>>#frameHasContext: */

static NoDbgRegParms int frameHasContext(char *theFP) {
  return (byteAt((theFP + FoxFrameFlags) + 2)) != 0;
}
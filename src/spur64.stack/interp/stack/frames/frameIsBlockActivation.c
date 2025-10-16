/* Extracted from interp.c:53975 (function frameIsBlockActivation). */

/*	<Integer> */

/* StackInterpreter>>#frameIsBlockActivation: */

static int frameIsBlockActivation(char *theFP) {
  return (byteAt((theFP + FoxFrameFlags) + 3)) != 0;
}
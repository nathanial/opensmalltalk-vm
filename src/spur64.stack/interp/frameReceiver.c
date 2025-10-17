/* Extracted from interp.c:54038 (function frameReceiver). */

/* StackInterpreter>>#frameReceiver: */

static NoDbgRegParms sqInt frameReceiver(char *theFP) {
  return longAt(theFP + FoxReceiver);
}
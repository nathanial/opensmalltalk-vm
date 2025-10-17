/* Extracted from interp.c:54772 (function iframeMethod). */

/* StackInterpreter>>#iframeMethod: */

static NoDbgRegParms usqInt iframeMethod(char *theFP) {
  return longAt(theFP + FoxMethod);
}
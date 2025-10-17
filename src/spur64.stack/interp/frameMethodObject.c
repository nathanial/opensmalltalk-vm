/* Extracted from interp.c:53985 (function frameMethodObject). */

/*	Homonym of frameMethod: for compatibility with CoInterpreter */

/* StackInterpreter>>#frameMethodObject: */

static NoDbgRegParms sqInt frameMethodObject(char *theFP) {
  return longAt(theFP + FoxMethod);
}
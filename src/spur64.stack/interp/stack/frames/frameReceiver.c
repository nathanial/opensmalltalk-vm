/* Extracted from interp.c:54038 (function frameReceiver). */

/* StackInterpreter>>#frameReceiver: */

static sqInt frameReceiver(char *theFP) { return longAt(theFP + FoxReceiver); }
/* Extracted from interp.c (function internalPush). */

/* StackInterpreter>>#internalPush: */

static void internalPush(char **localSPP, sqInt value) {
  longAtput(((*localSPP) -= BytesPerOop), value);
}

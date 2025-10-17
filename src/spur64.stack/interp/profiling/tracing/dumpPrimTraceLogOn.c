/* Extracted from interp.c:52139 (function dumpPrimTraceLogOn). */

/*	Print the prim trace log on a specific output stream. */
/*	essential for writing crash.dmp; use export: not api, so it will be
        accessible on win32 and won't be written to cointerp.h
 */

/* StackInterpreter>>#dumpPrimTraceLogOn: */

void dumpPrimTraceLogOn(FILE *aStdioStream) {
  FILE *savedTranscript;

  /* begin withRedirectedOutputTo:do: */
  savedTranscript = transcript;
  transcript = (aStdioStream ? aStdioStream : stdout);
  dumpPrimTraceLog();
  transcript = savedTranscript;
}
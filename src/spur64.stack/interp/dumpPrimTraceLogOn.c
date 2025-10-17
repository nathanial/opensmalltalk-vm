/* Extracted from interp.c:52139 (function dumpPrimTraceLogOn). */

/*	Print the prim trace log on a specific output stream. */
/*	essential for writing crash.dmp; use export: not api, so it will be
	accessible on win32 and won't be written to cointerp.h
 */

	/* StackInterpreter>>#dumpPrimTraceLogOn: */

void
dumpPrimTraceLogOn(FILE *aStdioStream)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    FILE *savedTranscript;

	/* begin withRedirectedOutputTo:do: */
	savedTranscript = GIV(transcript);
	GIV(transcript) = (aStdioStream
				? aStdioStream
				: stdout);
	dumpPrimTraceLog();
	GIV(transcript) = savedTranscript;
}
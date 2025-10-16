/* Extracted from interp.c:52139 (function dumpPrimTraceLogOn). */

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
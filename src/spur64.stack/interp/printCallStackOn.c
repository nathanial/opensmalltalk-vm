/* Extracted from interp.c:59542 (function printCallStackOn). */

void
printCallStackOn(FILE *aStdioStream)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    FILE *savedTranscript;

	/* begin withRedirectedOutputTo:do: */
	savedTranscript = GIV(transcript);
	GIV(transcript) = (aStdioStream
				? aStdioStream
				: stdout);
	printCallStack();
	GIV(transcript) = savedTranscript;
}
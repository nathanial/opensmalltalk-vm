/* Extracted from interp.c:59542 (function printCallStackOn). */

/*	Print the call stack on a specific output stream. */
/*	useful for VM debugging */

	/* StackInterpreter>>#printCallStackOn: */

void
printCallStackOn(FILE *aStdioStream)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    FILE *savedTranscript;

	/* begin withRedirectedOutputTo:do: */
	savedTranscript = transcript;
	transcript = (aStdioStream
				? aStdioStream
				: stdout);
	printCallStack();
	transcript = savedTranscript;
}
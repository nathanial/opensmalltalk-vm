/* Extracted from interp.c:59398 (function printAllStacksOn). */

/*	Print all the stacks of all running processes, including those that are
	currently suspended.
 */
/*	useful for VM debugging */

	/* StackInterpreter>>#printAllStacksOn: */

void
printAllStacksOn(FILE *aStdioStream)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    FILE *savedTranscript;

	/* begin withRedirectedOutputTo:do: */
	savedTranscript = GIV(transcript);
	GIV(transcript) = (aStdioStream
				? aStdioStream
				: stdout);
	printAllStacks();
	GIV(transcript) = savedTranscript;
}
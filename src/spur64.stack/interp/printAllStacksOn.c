/* Extracted from interp.c:59398 (function printAllStacksOn). */

/*	Print all the stacks of all running processes, including those that are
	currently suspended.
 */
/*	useful for VM debugging */

	/* StackInterpreter>>#printAllStacksOn: */

void
printAllStacksOn(FILE *aStdioStream)
{
    FILE *savedTranscript;

	/* begin withRedirectedOutputTo:do: */
	savedTranscript = transcript;
	transcript = (aStdioStream
				? aStdioStream
				: stdout);
	printAllStacks();
	transcript = savedTranscript;
}
/* Extracted from interp.c:52159 (function dumpTraceLog). */

/*	The trace log is a circular buffer of pairs of entries. If there is
	an entry at traceLogIndex - 3 \\ TraceBufferSize it has entries.
	If there is something at traceLogIndex it has wrapped. */

	/* StackInterpreter>>#dumpTraceLog */

void
dumpTraceLog(void)
{
    sqInt i;
    sqInt remainder;

	if (!(traceLog[/* safe:mod: */
				(((remainder = (traceLogIndex - 3) % TraceBufferSize)) < 0
					? remainder + TraceBufferSize
					: remainder)])) {
		return;
	}
	if (traceLog[traceLogIndex]) {
		for (i = traceLogIndex; i <= (TraceBufferSize - 3); i += 3) {
			printLogEntryAt(i);
		}
	}
	for (i = 0; i <= (traceLogIndex - 3); i += 3) {
		printLogEntryAt(i);
	}
}
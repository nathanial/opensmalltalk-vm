/* Extracted from interp.c:52159 (function dumpTraceLog). */

/*	The trace log is a circular buffer of pairs of entries. If there is
	an entry at traceLogIndex - 3 \\ TraceBufferSize it has entries.
	If there is something at traceLogIndex it has wrapped. */

	/* StackInterpreter>>#dumpTraceLog */

void
dumpTraceLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt remainder;

	if (!(GIV(traceLog)[/* safe:mod: */
				(((remainder = (GIV(traceLogIndex) - 3) % TraceBufferSize)) < 0
					? remainder + TraceBufferSize
					: remainder)])) {
		return;
	}
	if (GIV(traceLog)[GIV(traceLogIndex)]) {
		for (i = GIV(traceLogIndex); i <= (TraceBufferSize - 3); i += 3) {
			printLogEntryAt(i);
		}
	}
	for (i = 0; i <= (GIV(traceLogIndex) - 3); i += 3) {
		printLogEntryAt(i);
	}
}
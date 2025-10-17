/* Extracted from interp.c:57281 (function markAndTraceTraceLog). */

/*	The trace log is a circular buffer of pairs of entries. If there is an
	entry at
	traceLogIndex - 3 \\ TraceBufferSize it has entries. If there is something
	at traceLogIndex it has wrapped. */

	/* StackInterpreter>>#markAndTraceTraceLog */

static void
markAndTraceTraceLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt limit;
    sqInt oop;
    sqInt remainder;

	limit = /* safe:mod: */
			(((remainder = (traceLogIndex - 3) % TraceBufferSize)) < 0
				? remainder + TraceBufferSize
				: remainder);
	if (!(traceLog[limit])) {
		return;
	}
	if (traceLog[traceLogIndex]) {
		limit = TraceBufferSize - 3;
	}
	for (i = 0; i <= limit; i += 3) {
		oop = traceLog[i];
		if (!(((oop & (tagMask())) != 0))) {
			markAndTrace(oop);
		}
		oop = traceLog[i + 1];
		if (!(((oop & (tagMask())) != 0))) {
			markAndTrace(oop);
		}
	}
}
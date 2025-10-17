/* Extracted from interp.c:50941 (function checkLogIntegrity). */

/*	Check the log for leaks. The trace log is a circular buffer of pairs of
	entries. If there is an entry at traceLogIndex - 3 \\ TraceBufferSize it
	has entries. If
	there is something at traceLogIndex it has wrapped. */

	/* StackInterpreter>>#checkLogIntegrity */

static sqInt
checkLogIntegrity(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt limit;
    sqInt ok;
    sqInt oop;
    sqInt remainder;

	limit = /* safe:mod: */
			(((remainder = (GIV(traceLogIndex) - 3) % TraceBufferSize)) < 0
				? remainder + TraceBufferSize
				: remainder);
	if (!(GIV(traceLog)[limit])) {
		return 1;
	}
	if (GIV(traceLog)[GIV(traceLogIndex)]) {
		limit = TraceBufferSize - 3;
	}
	ok = 1;
	for (i = 0; i <= limit; i += 3) {
		oop = GIV(traceLog)[i];
		if (!(((oop & (tagMask())) != 0))) {
			if (!(checkOopIntegritynamedindex(oop, "traceLog", i))) {
				ok = 0;
			}
		}
		oop = GIV(traceLog)[i + 1];
		if (!(((oop & (tagMask())) != 0))) {
			if (!(checkOopIntegritynamedindex(oop, "traceLog", i + 1))) {
				ok = 0;
			}
		}
	}
	return ok;
}
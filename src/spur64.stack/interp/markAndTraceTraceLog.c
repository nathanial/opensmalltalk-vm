/* Extracted from interp.c:57281 (function markAndTraceTraceLog). */

static void
markAndTraceTraceLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt limit;
    sqInt oop;
    sqInt remainder;

	limit = /* safe:mod: */
			(((remainder = (GIV(traceLogIndex) - 3) % TraceBufferSize)) < 0
				? remainder + TraceBufferSize
				: remainder);
	if (!(GIV(traceLog)[limit])) {
		return;
	}
	if (GIV(traceLog)[GIV(traceLogIndex)]) {
		limit = TraceBufferSize - 3;
	}
	for (i = 0; i <= limit; i += 3) {
		oop = GIV(traceLog)[i];
		if (!(((oop & (tagMask())) != 0))) {
			markAndTrace(oop);
		}
		oop = GIV(traceLog)[i + 1];
		if (!(((oop & (tagMask())) != 0))) {
			markAndTrace(oop);
		}
	}
}
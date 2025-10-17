/* Extracted from interp.c:57050 (function mapTraceLog). */

/*	The trace log is a circular buffer of pairs of entries. If there is
	an entry at traceLogIndex - 3 \\ TraceBufferSize it has entries.
	If there is something at traceLogIndex it has wrapped. */

	/* StackInterpreter>>#mapTraceLog */

static void
mapTraceLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt intOrClass;
    sqInt limit;
    sqInt remainder;
    sqInt selectorOrMethod;

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
		intOrClass = traceLog[i];
		if (/* shouldRemapOop: */
			((!(intOrClass & (tagMask()))))
		 && (shouldRemapObj(intOrClass))) {
			traceLog[i] = (remapObj(intOrClass));
		}
		selectorOrMethod = traceLog[i + 1];
		if (/* shouldRemapOop: */
			((!(selectorOrMethod & (tagMask()))))
		 && (shouldRemapObj(selectorOrMethod))) {
			traceLog[i + 1] = (remapObj(selectorOrMethod));
		}
	}
}
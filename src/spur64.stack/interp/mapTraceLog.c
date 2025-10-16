/* Extracted from interp.c:57050 (function mapTraceLog). */

static void
mapTraceLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt intOrClass;
    sqInt limit;
    sqInt remainder;
    sqInt selectorOrMethod;

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
		intOrClass = GIV(traceLog)[i];
		if (/* shouldRemapOop: */
			((!(intOrClass & (tagMask()))))
		 && (shouldRemapObj(intOrClass))) {
			GIV(traceLog)[i] = (remapObj(intOrClass));
		}
		selectorOrMethod = GIV(traceLog)[i + 1];
		if (/* shouldRemapOop: */
			((!(selectorOrMethod & (tagMask()))))
		 && (shouldRemapObj(selectorOrMethod))) {
			GIV(traceLog)[i + 1] = (remapObj(selectorOrMethod));
		}
	}
}
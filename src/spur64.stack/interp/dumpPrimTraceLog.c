/* Extracted from interp.c:52108 (function dumpPrimTraceLog). */

void
dumpPrimTraceLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt hasParameter;
    sqInt i;
    sqInt remainder;

	if (!(GIV(primTraceLog)[/* safe:mod: */
				(((remainder = (GIV(primTraceLogIndex) - 1) % PrimTraceLogSize)) < 0
					? remainder + PrimTraceLogSize
					: remainder)])) {
		return;
	}
	hasParameter = 0;
	if (GIV(primTraceLog)[GIV(primTraceLogIndex)]) {
		for (i = GIV(primTraceLogIndex); i < PrimTraceLogSize; i += 1) {
			hasParameter = printPrimLogEntryAthasParameter(i, hasParameter);
		}
	}
	for (i = 0; i < GIV(primTraceLogIndex); i += 1) {
		hasParameter = printPrimLogEntryAthasParameter(i, hasParameter);
	}
}
/* Extracted from interp.c:52108 (function dumpPrimTraceLog). */

/*	The prim trace log is a circular buffer of entries. If there is
	an entry at primTraceLogIndex \\ PrimTraceLogSize it has entries.
	If there is something at primTraceLogIndex it has wrapped. */

	/* StackInterpreter>>#dumpPrimTraceLog */

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
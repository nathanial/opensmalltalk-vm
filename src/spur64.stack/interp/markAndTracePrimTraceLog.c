/* Extracted from interp.c:57091 (function markAndTracePrimTraceLog). */

static void
markAndTracePrimTraceLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt entryOop;
    sqInt i;
    sqInt remainder;

	if (!(GIV(primTraceLog)[/* safe:mod: */
				(((remainder = (GIV(primTraceLogIndex) - 1) % PrimTraceLogSize)) < 0
					? remainder + PrimTraceLogSize
					: remainder)])) {
		return;
	}
	if (GIV(primTraceLog)[GIV(primTraceLogIndex)]) {
		for (i = GIV(primTraceLogIndex); i < PrimTraceLogSize; i += 1) {
			entryOop = GIV(primTraceLog)[i];
			if ((entryOop != 0)
			 && ((!(entryOop & (tagMask()))))) {
				markAndTrace(entryOop);
			}
		}
	}
	for (i = 0; i < GIV(primTraceLogIndex); i += 1) {
		entryOop = GIV(primTraceLog)[i];
		if ((entryOop != 0)
		 && ((!(entryOop & (tagMask()))))) {
			markAndTrace(entryOop);
		}
	}
}
/* Extracted from interp.c:57091 (function markAndTracePrimTraceLog). */

/*	The prim trace log is a circular buffer of objects. If there is
	an entry at primTraceLogIndex - 1 \\ PrimTraceBufferSize it has entries.
	If there is something at primTraceLogIndex it has wrapped. */

	/* StackInterpreter>>#markAndTracePrimTraceLog */

static void
markAndTracePrimTraceLog(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt entryOop;
    sqInt i;
    sqInt remainder;

	if (!(primTraceLog[/* safe:mod: */
				(((remainder = (primTraceLogIndex - 1) % PrimTraceLogSize)) < 0
					? remainder + PrimTraceLogSize
					: remainder)])) {
		return;
	}
	if (primTraceLog[primTraceLogIndex]) {
		for (i = primTraceLogIndex; i < PrimTraceLogSize; i += 1) {
			entryOop = primTraceLog[i];
			if ((entryOop != 0)
			 && ((!(entryOop & (tagMask()))))) {
				markAndTrace(entryOop);
			}
		}
	}
	for (i = 0; i < primTraceLogIndex; i += 1) {
		entryOop = primTraceLog[i];
		if ((entryOop != 0)
		 && ((!(entryOop & (tagMask()))))) {
			markAndTrace(entryOop);
		}
	}
}
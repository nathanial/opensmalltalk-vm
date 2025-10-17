/* Extracted from interp.c:56915 (function mapPrimTraceLog). */

/*	The prim trace log is a circular buffer of objects. If there is
	an entry at primTraceLogIndex - 1 \\ PrimTraceBufferSize it has entries.
	If there is something at primTraceLogIndex it has wrapped. */

	/* StackInterpreter>>#mapPrimTraceLog */

static void
mapPrimTraceLog(void)
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
			 && (/* shouldRemapOop: */
				((!(entryOop & (tagMask()))))
			 && (shouldRemapObj(entryOop)))) {
				GIV(primTraceLog)[i] = (remapObj(entryOop));
			}
		}
	}
	for (i = 0; i < GIV(primTraceLogIndex); i += 1) {
		entryOop = GIV(primTraceLog)[i];
		if ((entryOop != 0)
		 && (/* shouldRemapOop: */
			((!(entryOop & (tagMask()))))
		 && (shouldRemapObj(entryOop)))) {
			GIV(primTraceLog)[i] = (remapObj(entryOop));
		}
	}
}
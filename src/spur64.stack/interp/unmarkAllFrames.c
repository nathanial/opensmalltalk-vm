/* Extracted from interp.c:72398 (function unmarkAllFrames). */

	/* StackInterpreterPrimitives>>#unmarkAllFrames */

static void
unmarkAllFrames(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt flags;
    sqInt i;
    char *theFP;
    StackPage *thePage;

	for (i = 0; i < numStackPages; i += 1) {
		/* begin stackPageAt: */
		thePage = stackPageAtpages(i, pages);
		if (!(isFree(thePage))) {
			theFP = (thePage->headFP);
			do {
				flags = longAt(theFP + FoxFrameFlags);
				if (flags & 2) {
					longAtput(theFP + FoxFrameFlags,flags - 2);
				}
			} while(((theFP = ((char *)(longAt(theFP + FoxSavedFP))))) != 0);
		}
	}
}
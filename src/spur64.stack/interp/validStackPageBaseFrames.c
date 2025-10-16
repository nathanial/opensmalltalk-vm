/* Extracted from interp.c:65598 (function validStackPageBaseFrames). */

static sqInt
validStackPageBaseFrames(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    StackPage *aPage;
    sqInt i;

	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		aPage = stackPageAtpages(i, GIV(pages));
		if (!(isFree(aPage))) {
			if (!(validStackPageBaseFrame(aPage))) {
				return 0;
			}
		}
	}
	return 1;
}
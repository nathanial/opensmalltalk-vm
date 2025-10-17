/* Extracted from interp.c:65598 (function validStackPageBaseFrames). */

/*	Check that the base frames in all in-use stack pages have a sender and a
	saved context.
 */

	/* StackInterpreter>>#validStackPageBaseFrames */

static sqInt
validStackPageBaseFrames(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    StackPage *aPage;
    sqInt i;

	for (i = 0; i < numStackPages; i += 1) {
		/* begin stackPageAt: */
		aPage = stackPageAtpages(i, pages);
		if (!(isFree(aPage))) {
			if (!(validStackPageBaseFrame(aPage))) {
				return 0;
			}
		}
	}
	return 1;
}
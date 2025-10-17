/* Extracted from interp.c:57315 (function markAndTraceUntracedReachableStackPages). */

/*	Trace any untraced pages */

	/* StackInterpreter>>#markAndTraceUntracedReachableStackPages */

static void
markAndTraceUntracedReachableStackPages(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt foundToBeTracedPage;
    sqInt i;
    StackPage *thePage;

	do {
		foundToBeTracedPage = 0;
		for (i = 0; i < GIV(numStackPages); i += 1) {
			/* begin stackPageAt: */
			thePage = stackPageAtpages(i, GIV(pages));
			if ((!(isFree(thePage)))
			 && (((thePage->trace)) == StackPageReachedButUntraced)) {
				foundToBeTracedPage = 1;
				markAndTraceStackPage(thePage);
			}
		}
	} while(foundToBeTracedPage);
}
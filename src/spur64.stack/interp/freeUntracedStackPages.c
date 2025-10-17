/* Extracted from interp.c:54062 (function freeUntracedStackPages). */

/*	Free any untraced stack pages. */

	/* StackInterpreter>>#freeUntracedStackPages */

static void
freeUntracedStackPages(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    StackPage *thePage;

	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		thePage = stackPageAtpages(i, GIV(pages));
		if ((!(isFree(thePage)))
		 && (((thePage->trace)) == StackPageUnreached)) {
			assert(noMarkedContextsOnPage(thePage));
			freeStackPage(thePage);
		}
		assert(((thePage->trace = StackPageTraceInvalid)) != 0);
	}
}
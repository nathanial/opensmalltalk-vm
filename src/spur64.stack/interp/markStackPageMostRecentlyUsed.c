/* Extracted from interp.c:10869 (function markStackPageMostRecentlyUsed). */

/*	<InterpreterStackPage> */
/*	MRUP-->used page<->used page<->used page<->used page<--LRUP
	^ <-next-prev-> ^
	| |
	v <-prev-next-> v
	free page<->free page<->free page<->free page */

	/* CogStackPages>>#markStackPageMostRecentlyUsed: */

static NoDbgRegParms sqInt
markStackPageMostRecentlyUsed(StackPage *page)
{
	if (page == mostRecentlyUsedPage) {
		return null;
	}

	/* Common case; making new page most recently used. */
	if (((page->prevPage)) == mostRecentlyUsedPage) {
		mostRecentlyUsedPage = page;
		assert(pageListIsWellFormed());
		return null;
	}
	(((page->prevPage))->nextPage = (page->nextPage));
	(((page->nextPage))->prevPage = (page->prevPage));
	(((mostRecentlyUsedPage->nextPage))->prevPage = page);
	(page->prevPage = mostRecentlyUsedPage);
	(page->nextPage = (mostRecentlyUsedPage->nextPage));
	(mostRecentlyUsedPage->nextPage = page);
	mostRecentlyUsedPage = page;
	assert(pageListIsWellFormed());
	return 0;
}
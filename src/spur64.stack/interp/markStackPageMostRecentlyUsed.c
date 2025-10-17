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
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (page == GIV(mostRecentlyUsedPage)) {
		return null;
	}

	/* Common case; making new page most recently used. */
	if (((page->prevPage)) == GIV(mostRecentlyUsedPage)) {
		GIV(mostRecentlyUsedPage) = page;
		assert(pageListIsWellFormed());
		return null;
	}
	(((page->prevPage))->nextPage = (page->nextPage));
	(((page->nextPage))->prevPage = (page->prevPage));
	(((GIV(mostRecentlyUsedPage)->nextPage))->prevPage = page);
	(page->prevPage = GIV(mostRecentlyUsedPage));
	(page->nextPage = (GIV(mostRecentlyUsedPage)->nextPage));
	(GIV(mostRecentlyUsedPage)->nextPage = page);
	GIV(mostRecentlyUsedPage) = page;
	assert(pageListIsWellFormed());
	return 0;
}
/* Extracted from interp.c:10907 (function markStackPageNextMostRecentlyUsed). */

/*	<InterpreterStackPage> */
/*	This method is used to move a page to a position in the list such that it
	cannot be deallocated when a new page is allocated, without changing the
	most recently
	used page. There must be at least 3 pages in the system. So making the
	page the MRU's prevPage is sufficient to ensure it won't be deallocated. */
/*	MRUP-->used page<->used page<->used page<->used page<--LRUP
	^ <-next-prev-> ^
	| |
	v <-prev-next-> v
	free page<->free page<->free page<->free page */

	/* CogStackPages>>#markStackPageNextMostRecentlyUsed: */

static NoDbgRegParms sqInt
markStackPageNextMostRecentlyUsed(StackPage *page)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(page != GIV(mostRecentlyUsedPage));
	if (((page->nextPage)) == GIV(mostRecentlyUsedPage)) {
		return null;
	}
	(((page->prevPage))->nextPage = (page->nextPage));
	(((page->nextPage))->prevPage = (page->prevPage));
	(((GIV(mostRecentlyUsedPage)->prevPage))->nextPage = page);
	(page->prevPage = (GIV(mostRecentlyUsedPage)->prevPage));
	(page->nextPage = GIV(mostRecentlyUsedPage));
	(GIV(mostRecentlyUsedPage)->prevPage = page);
	assert(pageListIsWellFormed());
	return 0;
}
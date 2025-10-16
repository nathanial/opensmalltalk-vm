/* Extracted from interp.c:10907 (function markStackPageNextMostRecentlyUsed). */

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
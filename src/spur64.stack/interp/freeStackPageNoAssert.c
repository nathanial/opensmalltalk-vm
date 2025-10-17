/* Extracted from interp.c:10820 (function freeStackPageNoAssert). */

/*	<InterpreterStackPage> */
/*	MRUP-->used page<->used page<->used page<->used page<--LRUP
	^ <-next-prev-> ^
	| |
	v <-prev-next-> v
	free page<->free page<->free page<->free page */

	/* CogStackPages>>#freeStackPageNoAssert: */

static NoDbgRegParms sqInt
freeStackPageNoAssert(StackPage *aPage)
{
    StackPage *prev;
    StackPage *self_in_CogStackPage;

	(aPage->baseFP = 0);
	if (aPage == mostRecentlyUsedPage) {
		mostRecentlyUsedPage = (mostRecentlyUsedPage->prevPage);
		return null;
	}
	self_in_CogStackPage = (prev = (aPage->prevPage));
	if (!((self_in_CogStackPage->baseFP))) {
		return null;
	}
	(prev->nextPage = (aPage->nextPage));
	(((aPage->nextPage))->prevPage = prev);
	(aPage->nextPage = (mostRecentlyUsedPage->nextPage));
	(((mostRecentlyUsedPage->nextPage))->prevPage = aPage);
	(aPage->prevPage = mostRecentlyUsedPage);
	(mostRecentlyUsedPage->nextPage = aPage);
	return 0;
}
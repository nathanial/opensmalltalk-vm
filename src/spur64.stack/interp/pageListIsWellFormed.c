/* Extracted from interp.c:10954 (function pageListIsWellFormed). */

/*	Answer if the stack page list is well-formed.
	MRUP-->used page<->used page<->used page<->used page<--LRUP
	^ <-next-prev-> ^
	| |
	v <-prev-next-> v
	free page<->free page<->free page<->free page */

	/* CogStackPages>>#pageListIsWellFormed */

static sqInt
pageListIsWellFormed(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt count;
    sqInt limit;
    sqInt ok;
    StackPage *page;

	ok = 1;
	page = (GIV(mostRecentlyUsedPage)->nextPage);
	count = 1;
	limit = GIV(numStackPages) * 2;
	while ((((page->baseFP)) == 0)
	 && ((page != GIV(mostRecentlyUsedPage))
	 && (count <= limit))) {
		if (!(asserta(((((page->nextPage))->prevPage)) == page))) {
			ok = 0;
		}
		page = (page->nextPage);
		count += 1;
	}
	while ((page != GIV(mostRecentlyUsedPage))
	 && (count <= limit)) {
		if (!(asserta(((((page->nextPage))->prevPage)) == page))) {
			ok = 0;
		}
		if (asserta(!(isFree(page)))) {
			if (!(asserta((addressIsInPage(page, (page->baseFP)))
				 && (addressIsInPage(page, (page->headSP)))))) {
				ok = 0;
			}
		}
		else {
			ok = 0;
		}
		page = (page->nextPage);
		count += 1;
	}
	if (!(asserta(count == (numStkPages())))) {
		ok = 0;
	}
	return ok;
}
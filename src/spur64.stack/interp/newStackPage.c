/* Extracted from interp.c:10932 (function newStackPage). */

/*	MRUP-->used page<->used page<->used page<->used page<--LRUP
	^ <-next-prev-> ^
	| |
	v <-prev-next-> v
	free page<->free page<->free page<->free page */

	/* CogStackPages>>#newStackPage */

static StackPage *
newStackPage(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    StackPage *lruOrFree;

	lruOrFree = (GIV(mostRecentlyUsedPage)->nextPage);
	if (!((lruOrFree->baseFP))) {
		return lruOrFree;
	}
	divorceFramesIn(lruOrFree);
	return lruOrFree;
}
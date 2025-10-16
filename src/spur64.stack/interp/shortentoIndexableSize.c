/* Extracted from interp.c:45546 (function shortentoIndexableSize). */

sqInt
shortentoIndexableSize(sqInt objOop, sqInt indexableSize)
{
    sqInt delta;

	assert((indexableSize >= 0)
	 && (indexableSize < (lengthOf(objOop))));
	delta = doShortentoIndexableSize(objOop, indexableSize);
	assert((lengthOf(followMaybeForwarded(objOop))) == indexableSize);

	/* Don't check if checking image segments, because we will check immediately after
	   the shorten in storeImageSegmentInto:outPointers:roots: */
	if (!(((checkForLeaks & GCModeImageSegment) != 0))) {
		runLeakCheckerFor(GCCheckShorten);
	}
	return delta;
}
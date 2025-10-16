/* Extracted from interp.c:45546 (function shortentoIndexableSize). */

/*	Reduce the number of indexable fields in objOop, an arrayFormat or
        longFormat object, to nSlots.
        Convert the unused residual to a free chunk (if in oldSpace).
        Answer the number of bytes returned to free memory, which may be zero.
 */

/* SpurMemoryManager>>#shorten:toIndexableSize: */

sqInt shortentoIndexableSize(sqInt objOop, sqInt indexableSize) {
  sqInt delta;

  assert((indexableSize >= 0) && (indexableSize < (lengthOf(objOop))));
  delta = doShortentoIndexableSize(objOop, indexableSize);
  assert((lengthOf(followMaybeForwarded(objOop))) == indexableSize);

  /* Don't check if checking image segments, because we will check immediately
     after the shorten in storeImageSegmentInto:outPointers:roots: */
  if (!(((checkForLeaks & GCModeImageSegment) != 0))) {
    runLeakCheckerFor(GCCheckShorten);
  }
  return delta;
}
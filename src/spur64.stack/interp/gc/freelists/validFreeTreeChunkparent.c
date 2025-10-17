/* Extracted from interp.c:46790 (function validFreeTreeChunkparent). */

/* SpurMemoryManager>>#validFreeTreeChunk:parent: */

static const char *validFreeTreeChunkparent(sqInt chunk, sqInt parent) {
  const char *reason;

  if (!chunk) {
    return null;
  }
  if (!(/* addressCouldBeOldObj: */
        ((chunk & (BaseHeaderSize - 1)) == 0) &&
        (oopisGreaterThanOrEqualToandLessThan(chunk, oldSpaceStart,
                                              endOfMemory)))) {
    return "not in old space";
  }
  if (((bytesInBody(chunk)) / 8 /* allocationUnit */) < 64 /* numFreeLists */) {
    return "too small";
  }
  if (parent !=
      (fetchPointerofObject(2U, chunk))) {
    return "bad parent";
  }
  if ((segmentContainingObj(chunk)) !=
      (segmentContainingObj(addressAfter(chunk)))) {
    return "not in one segment";
  }
  if ((reason =
           validFreeTreeChunkparent(fetchPointerofObject(3U, chunk),
                                    chunk))) {
    return reason;
  }
  if ((reason =
           validFreeTreeChunkparent(fetchPointerofObject(4U, chunk),
                                    chunk))) {
    return reason;
  }
  return null;
}
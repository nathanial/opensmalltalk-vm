/* Extracted from interp.c:46497 (function unlinkLilliputianChunkindex). */

/* SpurMemoryManager>>#unlinkLilliputianChunk:index: */

static NeverInline sqInt unlinkLilliputianChunkindex(sqInt freeChunk,
                                                     sqInt index) {
  sqInt next;
  sqInt nextSqInt;
  sqInt node;
  sqInt prev;

  /* for profiling */
  node = freeLists[index];
  prev = 0;
  while (node != 0) {
    assert(node == (startOfObject(node)));

    /* begin assertValidFreeObject: */
    assert(assertInnerValidFreeObject(node));
    next = fetchPointerofObject(0U, node);
    if (node == freeChunk) {
      if (prev) {
        /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
        /* begin storePointer:ofFreeChunk:withValue: */
        assert(isFreeObject(prev));
        assert((next == 0) || (isFreeObject(next)));
        longAtput((void *)((prev + BaseHeaderSize) + (0U << (shiftForWord()))),
                  next);
      } else {
        /* begin unlinkFreeChunk:atIndex:isLilliputianSize: */
        assert(((bytesInBody(freeChunk)) == (index * (allocationUnit()))) &&
               ((index > 1) && ((startOfObject(freeChunk)) == freeChunk)));

        /* For some reason the assertion is not compiled correctly */
        freeLists[index] = ((nextSqInt = fetchPointerofObject(0U, freeChunk)));
      }
      return freeChunk;
    }
    prev = node;
    node = next;
  }
  error("freeChunk not found in lilliputian chunk free list");
  return 0;
}
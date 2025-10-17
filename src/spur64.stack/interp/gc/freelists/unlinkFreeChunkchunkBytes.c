/* Extracted from interp.c:46416 (function unlinkFreeChunkchunkBytes). */

/*	Unlink a free object from the free lists. Do not alter
   totalFreeOldSpace. Used for coalescing.
 */

/* SpurMemoryManager>>#unlinkFreeChunk:chunkBytes: */

static sqInt unlinkFreeChunkchunkBytes(sqInt freeChunk, sqInt chunkBytes) {
  sqInt index;
  sqInt lilliputian;
  sqInt next;
  sqInt nextFreeChunk;
  sqInt nextSqInt;
  sqInt prev;

  index = chunkBytes / 8 /* allocationUnit */;

  /* Pathological 64 bits case - size 1 - single linked list */

  /* begin isLilliputianSize: */
  assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
  if (chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */)) {
    return unlinkLilliputianChunkindex(freeChunk, index);
  }
  prev = fetchPointerofObject(1U, freeChunk);

  /* Has prev element: update double linked list */
  if (prev) {
    nextFreeChunk = fetchPointerofObject(0U, freeChunk);

    /* begin setNextFreeChunkOf:withValue:chunkBytes: */
    /* begin isLilliputianSize: */
    assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
    lilliputian = chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */);

    /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
    /* begin storePointer:ofFreeChunk:withValue: */
    assert(isFreeObject(prev));
    assert((nextFreeChunk == 0) || (isFreeObject(nextFreeChunk)));
    longAtput((void *)((prev + BaseHeaderSize) + (0U << (shiftForWord()))),
              nextFreeChunk);
    if ((nextFreeChunk != 0) && (!lilliputian)) {
      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(nextFreeChunk));
      assert((prev == 0) || (isFreeObject(prev)));
      longAtput(
          (void *)((nextFreeChunk + BaseHeaderSize) + (1U << (shiftForWord()))),
          prev);
    }
    return freeChunk;
  }

  /* Is the beginning of a list
     Small chunk */
  if ((index < 64 /* numFreeLists */) && ((1ULL << index) <= freeListsMask)) {
    /* begin unlinkFreeChunk:atIndex:isLilliputianSize: */
    assert(((bytesInBody(freeChunk)) == (index * (allocationUnit()))) &&
           ((index > 1) && ((startOfObject(freeChunk)) == freeChunk)));

    /* For some reason the assertion is not compiled correctly */
    freeLists[index] = ((nextSqInt = fetchPointerofObject(0U, freeChunk)));
    if (nextSqInt) {
      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(nextSqInt));
      longAtput(
          (void *)((nextSqInt + BaseHeaderSize) + (1U << (shiftForWord()))), 0);
    }
    return freeChunk;
  }

  /* Large chunk */
  next = fetchPointerofObject(0U, freeChunk);
  if (next) {
    inFreeTreeReplacewith(freeChunk, next);
  } else {
    unlinkSolitaryFreeTreeNode(freeChunk);
  }

  /* no list; remove the interior node
     list; replace node with it */
  return freeChunk;
}
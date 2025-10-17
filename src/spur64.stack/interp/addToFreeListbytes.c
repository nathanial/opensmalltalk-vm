/* Extracted from interp.c:29772 (function addToFreeListbytes). */

/*	Add freeChunk to the relevant freeList.
        For the benefit of sortedFreeObject:, if freeChunk is large, answer the
        treeNode it
        is added to, if it is added to the next list of a freeTreeNode,
   otherwise answer 0. */
/*	coInterpreter transcript ensureCr. coInterpreter print: 'freeing '. self
        printFreeChunk: freeChunk.
 */

/* SpurMemoryManager>>#addToFreeList:bytes: */

static NoDbgRegParms sqInt addToFreeListbytes(sqInt freeChunk,
                                              sqInt chunkBytes) {
  sqInt child;
  usqInt childBytes;
  sqInt index;
  sqInt lilliputian;
  sqInt nextFreeChunk;
  sqInt parent;

  assert(isFreeObject(freeChunk));
  assert(chunkBytes == (bytesInBody(freeChunk)));

  /* Too slow to be enabled byt default but useful to debug Selective...
     self deny: (compactor isSegmentBeingCompacted: (segmentManager
     segmentContainingObj: freeChunk)). */
  index = chunkBytes / 8 /* allocationUnit */;
  if (index < 64 /* numFreeLists */) {
    nextFreeChunk = freeLists[index];

    /* begin setNextFreeChunkOf:withValue:chunkBytes: */
    /* begin isLilliputianSize: */
    assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
    lilliputian = chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */);

    /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
    /* begin storePointer:ofFreeChunk:withValue: */
    assert(isFreeObject(freeChunk));
    assert((nextFreeChunk == 0) || (isFreeObject(nextFreeChunk)));
    longAtput((void *)((freeChunk + BaseHeaderSize) + (0U << (shiftForWord()))),
              nextFreeChunk);
    if ((nextFreeChunk != 0) && (!lilliputian)) {
      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(nextFreeChunk));
      assert((freeChunk == 0) || (isFreeObject(freeChunk)));
      longAtput(
          (void *)((nextFreeChunk + BaseHeaderSize) + (1U << (shiftForWord()))),
          freeChunk);
    }

    /* begin isLilliputianSize: */
    assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
    if (!(chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */))) {
      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(freeChunk));
      longAtput(
          (void *)((freeChunk + BaseHeaderSize) + (1U << (shiftForWord()))), 0);
    }
    freeLists[index] = freeChunk;
    freeListsMask = freeListsMask | (1ULL << index);
    return 0;
  }

  /* begin addToFreeTree:bytes: */
  /* begin initFreeTreeChunk:bytes: */
  assert(isFreeObject(freeChunk));
  assert(chunkBytes == (bytesInBody(freeChunk)));
  assert(chunkBytes >= ((numFreeLists()) * (allocationUnit())));

  /* begin storePointer:ofFreeChunk:withValue: */
  assert(isFreeObject(freeChunk));
  longAtput((void *)((freeChunk + BaseHeaderSize) + (0U << (shiftForWord()))),
            0);

  /* begin storePointer:ofFreeChunk:withValue: */
  assert(isFreeObject(freeChunk));
  longAtput((void *)((freeChunk + BaseHeaderSize) + (1U << (shiftForWord()))),
            0);

  /* begin storePointer:ofFreeChunk:withValue: */
  assert(isFreeObject(freeChunk));
  longAtput((void *)((freeChunk + BaseHeaderSize) + (2U << (shiftForWord()))),
            0);

  /* begin storePointer:ofFreeChunk:withValue: */
  assert(isFreeObject(freeChunk));
  longAtput((void *)((freeChunk + BaseHeaderSize) + (3U << (shiftForWord()))),
            0);

  /* begin storePointer:ofFreeChunk:withValue: */
  assert(isFreeObject(freeChunk));
  longAtput((void *)((freeChunk + BaseHeaderSize) + (4U << (shiftForWord()))),
            0);

  /* Large chunk list organized as a tree, each node of which is a list of
     chunks of the same size. Beneath the node are smaller and larger blocks. */
  parent = 0;
  child = freeLists[0];
  while (child != 0) {
    childBytes = bytesInBody(child);

    /* check for overlap; could write this as self oop: (self objectAfter:
       freeChunk) isLessThanOrEqualTo: child... but that relies on headers being
       correct, etc.  So keep it clumsy... */
    assert((oopisLessThanOrEqualTo((freeChunk + chunkBytes) - BaseHeaderSize,
                                   child)) ||
           (oopisGreaterThanOrEqualTo(freeChunk,
                                      (child + childBytes) - BaseHeaderSize)));
    if (childBytes == chunkBytes) {
      nextFreeChunk =
          longAt((void *)((child + BaseHeaderSize) + (0U << (shiftForWord()))));

      /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(freeChunk));
      assert((nextFreeChunk == 0) || (isFreeObject(nextFreeChunk)));
      longAtput(
          (void *)((freeChunk + BaseHeaderSize) + (0U << (shiftForWord()))),
          nextFreeChunk);
      if (nextFreeChunk) {
        /* begin storePointer:ofFreeChunk:withValue: */
        assert(isFreeObject(nextFreeChunk));
        assert((freeChunk == 0) || (isFreeObject(freeChunk)));
        longAtput((void *)((nextFreeChunk + BaseHeaderSize) +
                           (1U << (shiftForWord()))),
                  freeChunk);
      }

      /* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(child));
      assert((freeChunk == 0) || (isFreeObject(freeChunk)));
      longAtput((void *)((child + BaseHeaderSize) + (0U << (shiftForWord()))),
                freeChunk);
      if (freeChunk) {
        /* begin storePointer:ofFreeChunk:withValue: */
        assert(isFreeObject(freeChunk));
        assert((child == 0) || (isFreeObject(child)));
        longAtput(
            (void *)((freeChunk + BaseHeaderSize) + (1U << (shiftForWord()))),
            child);
      }
      return child;
    }

    /* size match; add to list at node.
       walk down the tree */
    parent = child;
    child = longAt((void *)((child + BaseHeaderSize) +
                            ((((usqInt)(((childBytes > chunkBytes
                                              ? 3 /* freeChunkSmallerIndex */
                                              : 4 /* freeChunkLargerIndex */)))
                               << (shiftForWord()))))));
  }
  if (!parent) {
    assert((freeLists[0]) == 0);
    freeLists[0] = freeChunk;
    freeListsMask = freeListsMask | 1;
    return 0;
  }
  assert(((freeListsMask & 1) != 0));

  /* insert in tree */

  /* begin storePointer:ofFreeChunk:withValue: */
  assert(isFreeObject(freeChunk));
  assert((parent == 0) || (isFreeObject(parent)));
  longAtput((void *)((freeChunk + BaseHeaderSize) + (2U << (shiftForWord()))),
            parent);

  /* begin storePointer:ofFreeChunk:withValue: */
  assert(isFreeObject(parent));
  assert((freeChunk == 0) || (isFreeObject(freeChunk)));
  longAtput((void *)((parent + BaseHeaderSize) +
                     ((((usqInt)(((childBytes > chunkBytes
                                       ? 3 /* freeChunkSmallerIndex */
                                       : 4 /* freeChunkLargerIndex */)))
                        << (shiftForWord()))))),
            freeChunk);
  return 0;
}
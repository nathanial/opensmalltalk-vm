/* Extracted from interp.c:36333 (function inFreeTreeReplacewith). */

/*	Part of reorderReversedTreeList:. Switch treeNode with newNode in
        the tree, but do nothing to the list linked through freeChunkNextIndex.
 */

/* SpurMemoryManager>>#inFreeTreeReplace:with: */

static NoDbgRegParms void inFreeTreeReplacewith(sqInt treeNode, sqInt newNode) {
  sqInt fieldIndex;
  sqInt i;
  sqInt relative;

  /* begin storePointer:ofFreeChunk:withValue: */
  assert(isFreeObject(newNode));
  longAtput((void *)((newNode + BaseHeaderSize) + (1U << (shiftForWord()))), 0);

  /* copy parent, smaller, larger */
  for (i = 2 /* freeChunkParentIndex */; i <= 4 /* freeChunkLargerIndex */;
       i += 1) {
    relative = longAt((void *)((treeNode + BaseHeaderSize) +
                               ((((usqInt)(i) << (shiftForWord()))))));
    if (i == 2 /* freeChunkParentIndex */) {
      if (relative) {
        fieldIndex = (treeNode == (longAt((void *)((relative + BaseHeaderSize) +
                                                   (3U << (shiftForWord())))))
                          ? 3 /* freeChunkSmallerIndex */
                          : 4 /* freeChunkLargerIndex */);

        /* begin storePointer:ofFreeChunk:withValue: */
        assert(isFreeObject(relative));
        assert((newNode == 0) || (isFreeObject(newNode)));
        longAtput((void *)((relative + BaseHeaderSize) +
                           ((((usqInt)(fieldIndex) << (shiftForWord()))))),
                  newNode);
      } else {
        assert((freeLists[0]) == treeNode);
        freeLists[0] = newNode;
      }
    } else {
      if (relative) {
        assert((fetchPointerofFreeChunk(freeChunkParentIndex(), relative)) ==
               treeNode);

        /* begin storePointer:ofFreeChunk:withValue: */
        assert(isFreeObject(relative));
        assert((newNode == 0) || (isFreeObject(newNode)));
        longAtput(
            (void *)((relative + BaseHeaderSize) + (2U << (shiftForWord()))),
            newNode);
      }
    }

    /* begin storePointer:ofFreeChunk:withValue: */
    assert(isFreeObject(newNode));
    assert((relative == 0) || (isFreeObject(relative)));
    longAtput((void *)((newNode + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              relative);

    /* begin storePointer:ofFreeChunk:withValue: */
    assert(isFreeObject(treeNode));
    longAtput((void *)((treeNode + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              0);
  }
}
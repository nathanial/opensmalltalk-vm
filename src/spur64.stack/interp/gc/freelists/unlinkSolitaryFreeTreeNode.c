/* Extracted from interp.c:46546 (function unlinkSolitaryFreeTreeNode). */

/*	Unlink a freeTreeNode. Assumes the node has no list (null next link). */

/* SpurMemoryManager>>#unlinkSolitaryFreeTreeNode: */

static void unlinkSolitaryFreeTreeNode(sqInt freeTreeNode) {
  sqInt fieldIndex;
  sqInt larger;
  sqInt parent;
  sqInt smaller;

  assert((fetchPointerofFreeChunk(freeChunkNextIndex(), freeTreeNode)) == 0);

  /* case 1. interior node has one child, P = parent, N = node, S = subtree
     (mirrored for large vs small)
     ___				  ___
     | P |				  | P |
     _/_				_/_
     | N |		=>		| S |
     _/_
     | S |
     case 2: interior node has two children, , P = parent, N = node, L =
     smaller, left subtree, R = larger, right subtree. add the left subtree to
     the bottom left of the right subtree (mirrored for large vs small)
     ___				  ___
     | P |				  | P |
     _/_				_/_
     | N |		=>		| R |
     _/_  _\_		    _/_
     | L | | R |		    | L | */
  smaller = longAt(
      (void *)((freeTreeNode + BaseHeaderSize) + (3U << (shiftForWord()))));
  larger = longAt(
      (void *)((freeTreeNode + BaseHeaderSize) + (4U << (shiftForWord()))));
  parent = longAt(
      (void *)((freeTreeNode + BaseHeaderSize) + (2U << (shiftForWord()))));
  if (parent) {
    if (smaller) {
      fieldIndex = (freeTreeNode == (longAt((void *)((parent + BaseHeaderSize) +
                                                     (3U << (shiftForWord())))))
                        ? 3 /* freeChunkSmallerIndex */
                        : 4 /* freeChunkLargerIndex */);

      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(parent));
      assert((smaller == 0) || (isFreeObject(smaller)));
      longAtput((void *)((parent + BaseHeaderSize) +
                         ((((usqInt)(fieldIndex) << (shiftForWord()))))),
                smaller);

      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(smaller));
      assert((parent == 0) || (isFreeObject(parent)));
      longAtput((void *)((smaller + BaseHeaderSize) + (2U << (shiftForWord()))),
                parent);
      if (larger) {
        addFreeSubTree(larger);
      }
    } else {
      fieldIndex = (freeTreeNode == (longAt((void *)((parent + BaseHeaderSize) +
                                                     (3U << (shiftForWord())))))
                        ? 3 /* freeChunkSmallerIndex */
                        : 4 /* freeChunkLargerIndex */);

      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(parent));
      assert((larger == 0) || (isFreeObject(larger)));
      longAtput((void *)((parent + BaseHeaderSize) +
                         ((((usqInt)(fieldIndex) << (shiftForWord()))))),
                larger);
      if (larger) {
        /* begin storePointer:ofFreeChunk:withValue: */
        assert(isFreeObject(larger));
        assert((parent == 0) || (isFreeObject(parent)));
        longAtput(
            (void *)((larger + BaseHeaderSize) + (2U << (shiftForWord()))),
            parent);
      }
    }
  } else {
    if (smaller) {
      /* begin storePointer:ofFreeChunk:withValue: */
      assert(isFreeObject(smaller));
      longAtput((void *)((smaller + BaseHeaderSize) + (2U << (shiftForWord()))),
                0);
      freeLists[0] = smaller;
      if (larger) {
        addFreeSubTree(larger);
      }
    } else {
      if (larger) {
        /* begin storePointer:ofFreeChunk:withValue: */
        assert(isFreeObject(larger));
        longAtput(
            (void *)((larger + BaseHeaderSize) + (2U << (shiftForWord()))), 0);
      }
      freeLists[0] = larger;
    }
  }
}
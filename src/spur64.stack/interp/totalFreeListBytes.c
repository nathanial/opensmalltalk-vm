/* Extracted from interp.c:46275 (function totalFreeListBytes). */

/*	This method both computes the actual number of free bytes by traversing
        all free objects
        on the free lists/tree, and checks that the tree is valid. It is used
        mainly by checkFreeSpace. */

/* SpurMemoryManager>>#totalFreeListBytes */

static sqInt totalFreeListBytes(void) {
  sqInt bytesInChunk;
  sqInt cameFrom;
  sqInt i;
  sqInt largeChild;
  sqInt listNode;
  sqInt nextNode;
  sqInt smallChild;
  sqInt totalFreeBytes;
  sqInt treeNodeSqInt;

  totalFreeBytes = 0;
  for (i = 1; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
    bytesInChunk = i * 8 /* allocationUnit */;
    listNode = freeLists[i];
    while (listNode != 0) {
      totalFreeBytes += bytesInChunk;

      /* begin assertValidFreeObject: */
      assert(assertInnerValidFreeObject(listNode));
      assert(bytesInChunk == (bytesInBody(listNode)));
      nextNode = longAt(
          (void *)((listNode + BaseHeaderSize) + (0U << (shiftForWord()))));
      assert(nextNode != listNode);
      listNode = nextNode;
    }
  }

  /* begin freeTreeNodesDo: */
  treeNodeSqInt = freeLists[0];
  if (!treeNodeSqInt) {
    goto l1;
  }
  cameFrom = -1;
  do {
    assert((bytesInBody(treeNodeSqInt)) >=
           ((numFreeLists()) * (allocationUnit())));
    smallChild = longAt(
        (void *)((treeNodeSqInt + BaseHeaderSize) + (3U << (shiftForWord()))));
    largeChild = longAt(
        (void *)((treeNodeSqInt + BaseHeaderSize) + (4U << (shiftForWord()))));
    assert((smallChild == 0) ||
           (treeNodeSqInt ==
            (fetchPointerofFreeChunk(freeChunkParentIndex(), smallChild))));
    assert((largeChild == 0) ||
           (treeNodeSqInt ==
            (fetchPointerofFreeChunk(freeChunkParentIndex(), largeChild))));

    /* apply if the node has no children, or it has no large children and we're
       returning from the small child, or we're returning from the large child.
     */
    if (((smallChild == 0) && (largeChild == 0)) ||
        ((largeChild ? cameFrom == largeChild : cameFrom == smallChild))) {
      bytesInChunk = bytesInBody(treeNodeSqInt);
      assert((bytesInChunk / (allocationUnit())) >= (numFreeLists()));
      listNode = treeNodeSqInt;
      while (listNode != 0) {
        /* begin assertValidFreeObject: */
        assert(assertInnerValidFreeObject(listNode));
        assert(
            (listNode == treeNodeSqInt) ||
            ((fetchPointerofFreeChunk(freeChunkParentIndex(), listNode)) == 0));
        totalFreeBytes += bytesInChunk;
        assert(bytesInChunk == (bytesInBody(listNode)));
        nextNode = longAt(
            (void *)((listNode + BaseHeaderSize) + (0U << (shiftForWord()))));
        assert(nextNode != listNode);
        listNode = nextNode;
      }

      /* self printFreeChunk: listNode */

      /* and since we've applied we must move on up */
      cameFrom = treeNodeSqInt;
      treeNodeSqInt = longAt((void *)((treeNodeSqInt + BaseHeaderSize) +
                                      (2U << (shiftForWord()))));
    } else {
      if ((smallChild != 0) && (cameFrom != smallChild)) {
        treeNodeSqInt = smallChild;
      } else {
        assert(largeChild != 0);
        treeNodeSqInt = largeChild;
      }
      cameFrom = -1;
    }
  } while (treeNodeSqInt != 0);
  /* end freeTreeNodesDo: */
l1:
  return totalFreeBytes;
}
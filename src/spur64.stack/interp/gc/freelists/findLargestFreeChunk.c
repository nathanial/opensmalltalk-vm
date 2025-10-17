/* Extracted from interp.c:35114 (function findLargestFreeChunk). */

/*	Answer, but do not remove, the largest free chunk in the free lists. */

/* SpurMemoryManager>>#findLargestFreeChunk */

static sqInt findLargestFreeChunk(void) {
  sqInt childNode;
  sqInt treeNode;

  childNode = 0;
  treeNode = freeLists[0];
  if (!treeNode) {
    return null;
  }
  while (1) {
    /* begin assertValidFreeObject: */
    assert(assertInnerValidFreeObject(treeNode));
    assert((bytesInBody(treeNode)) >= ((numFreeLists()) * (allocationUnit())));
    childNode = fetchPointerofObject(4U, treeNode);
    if (!(childNode != 0))
      break;
    treeNode = childNode;
  }
  return treeNode;
}
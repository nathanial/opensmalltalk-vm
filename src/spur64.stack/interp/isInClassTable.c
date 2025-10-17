/* Extracted from interp.c:37333 (function isInClassTable). */

/* SpurMemoryManager>>#isInClassTable: */

static sqInt isInClassTable(sqInt objOop) {
  sqInt hash;

  hash = (long32At((void *)(objOop + 4))) & (identityHashHalfWordMask());
  return (hash != 0) && ((classAtIndex(hash)) == objOop);
}
/* Extracted from interp.c:46780 (function validFreeTreeChunk). */

/* SpurMemoryManager>>#validFreeTreeChunk: */

static sqInt validFreeTreeChunk(sqInt chunk) {
  if (!(segmentContainingObj(chunk))) {
    return 0;
  }
  return !(validFreeTreeChunkparent(
      chunk,
      fetchPointerofObject(2U, chunk)));
}
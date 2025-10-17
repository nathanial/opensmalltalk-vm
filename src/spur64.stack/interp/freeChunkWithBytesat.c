/* Extracted from interp.c:35815 (function freeChunkWithBytesat). */

/* SpurMemoryManager>>#freeChunkWithBytes:at: */

static NoDbgRegParms sqInt freeChunkWithBytesat(sqInt bytes, sqInt address) {
  sqInt freeChunk;

  assert(isInOldSpace(address));
  assert((segmentContainingObj(address)) ==
         (segmentContainingObj(address + bytes)));
  freeChunk = initFreeChunkWithBytesat(bytes, address);
  addToFreeListbytes(freeChunk, bytes);
  assert(freeChunk == (objectStartingAt(address)));
  return freeChunk;
}
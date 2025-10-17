/* Extracted from interp.c:35069 (function fetchPointerofFreeChunk). */

/* SpurMemoryManager>>#fetchPointer:ofFreeChunk: */

static NoDbgRegParms sqInt fetchPointerofFreeChunk(sqInt fieldIndex,
                                                   sqInt objOop) {
  return longAt((void *)((objOop + BaseHeaderSize) +
                         ((((usqInt)(fieldIndex) << (shiftForWord()))))));
}
/* Extracted from interp.c:34992 (function fetchByteofObject). */

/* SpurMemoryManager>>#fetchByte:ofObject: */

sqInt fetchByteofObject(sqInt byteIndex, sqInt objOop) {
  return byteAt((void *)((objOop + BaseHeaderSize) + byteIndex));
}
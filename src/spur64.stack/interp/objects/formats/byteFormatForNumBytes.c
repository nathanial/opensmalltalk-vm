/* Extracted from interp.c:31782 (function byteFormatForNumBytes). */

/* SpurMemoryManager>>#byteFormatForNumBytes: */

static sqInt byteFormatForNumBytes(sqInt numBytes) {
  return (firstByteFormat()) + ((8 - numBytes) & (BytesPerWord - 1));
}
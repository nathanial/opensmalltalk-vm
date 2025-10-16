/* Extracted from interp.c:31839 (function bytesPerElement). */

/*	Answer the basic element size for the receiver. Answer 0 for immediates
        and CompiledCode
        (element size could be wordSize for literals or 1 for bytes, so its
        indeterminable). Answer
        wordSize for pointer objects. Otherwise answer the actual element size
   of a bits container. */

/* SpurMemoryManager>>#bytesPerElement: */

unsigned char bytesPerElement(sqInt oop) {
  unsigned char byteSizes[16] = {
      wordSize(), wordSize(), wordSize(),
      wordSize(), wordSize(), wordSize(),
      wordSize(), 0,          /* forwarder */ wordSize(),
      8,          4,          4,
      2,          2,          2,
      2};
  sqInt fmt;

  if (((oop & (tagMask())) != 0)) {
    return 0;
  }
  fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
  if (fmt >= (firstByteFormat())) {
    if (fmt >= (firstCompiledMethodFormat())) {
      return 0;
    }
    return 1;
  }
  return byteSizes[fmt];
}
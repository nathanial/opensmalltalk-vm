/* Extracted from interp.c:37827 (function isWeakNonImm). */

/* SpurMemoryManager>>#isWeakNonImm: */

static NoDbgRegParms int isWeakNonImm(sqInt objOop) {
  sqInt format;

  format =
      (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

  /* begin isWeakFormat: */
  return format == (weakArrayFormat());
}
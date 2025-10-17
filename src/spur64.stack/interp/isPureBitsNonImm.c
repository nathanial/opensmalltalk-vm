/* Extracted from interp.c:37579 (function isPureBitsNonImm). */

/*	Answer if the argument contains only indexable words (no oops). See
        comment in formatOf:
 */

/* SpurMemoryManager>>#isPureBitsNonImm: */

static NoDbgRegParms sqInt isPureBitsNonImm(sqInt objOop) {
  sqInt format;

  format =
      (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

  /* begin isPureBitsFormat: */
  return (format >= (sixtyFourBitIndexableFormat())) &&
         (format < (firstCompiledMethodFormat()));
}
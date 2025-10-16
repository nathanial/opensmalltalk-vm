/* Extracted from interp.c:37858 (function isWordsOrBytesNonImm). */

/*	Answer if the contains only indexable words or bytes (no oops). See
        comment in formatOf:
 */
/*	Note: Excludes CompiledMethods. */

/* SpurMemoryManager>>#isWordsOrBytesNonImm: */

static sqInt isWordsOrBytesNonImm(sqInt objOop) {
  sqInt format;

  format =
      (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

  /* begin isPureBitsFormat: */
  return (format >= (sixtyFourBitIndexableFormat())) &&
         (format < (firstCompiledMethodFormat()));
}
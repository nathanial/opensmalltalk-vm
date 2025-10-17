/* Extracted from interp.c:36130 (function hasPointerFieldsNonImm). */

/* SpurMemoryManager>>#hasPointerFieldsNonImm: */

static sqInt hasPointerFieldsNonImm(sqInt oop) {
  sqInt format;

  format = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());

  /* begin isAnyPointerFormat: */
  return (format <= 5 /* lastPointerFormat */) ||
         (format >= (firstCompiledMethodFormat()));
}
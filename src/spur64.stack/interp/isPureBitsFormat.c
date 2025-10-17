/* Extracted from interp.c:37566 (function isPureBitsFormat). */

/*	the inverse of isAnyPointerFormat: */

/* SpurMemoryManager>>#isPureBitsFormat: */

static NoDbgRegParms sqInt isPureBitsFormat(sqInt format) {
  return (format >= (sixtyFourBitIndexableFormat())) &&
         (format < (firstCompiledMethodFormat()));
}
/* Extracted from interp.c:37511 (function isOopCompiledMethod). */

/*	Answer whether the oop is an object of compiled method format */

/* SpurMemoryManager>>#isOopCompiledMethod: */

sqInt isOopCompiledMethod(sqInt oop) {
  return ((!(oop & (tagMask())))) &&
         (((byteAt((void *)(oop + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstCompiledMethodFormat()));
}
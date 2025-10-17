/* Extracted from interp.c:37192 (function isCompiledMethod). */

/*	Answer whether the argument object is of compiled method format */

/* SpurMemoryManager>>#isCompiledMethod: */

int isCompiledMethod(sqInt objOop) {
  return ((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstCompiledMethodFormat());
}
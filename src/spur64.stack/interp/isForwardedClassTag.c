/* Extracted from interp.c:37286 (function isForwardedClassTag). */

/* SpurMemoryManager>>#isForwardedClassTag: */

static NoDbgRegParms int isForwardedClassTag(sqInt classIndex) {
  return classIndex == (isForwardedObjectClassIndexPun());
}
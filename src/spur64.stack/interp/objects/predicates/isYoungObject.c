/* Extracted from interp.c:37914 (function isYoungObject). */

/*	Answer if obj is young. Require that obj is non-immediate. */

/* SpurMemoryManager>>#isYoungObject: */

int isYoungObject(sqInt objOop) {
  assert(isNonImmediate(objOop));
  return oopisLessThan(objOop, oldSpaceStart);
}
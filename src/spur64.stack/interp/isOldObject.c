/* Extracted from interp.c:37500 (function isOldObject). */

/*	Answer if obj is old. Require that obj is non-immediate. */

/* SpurMemoryManager>>#isOldObject: */

static int isOldObject(sqInt objOop) {
  assert(isNonImmediate(objOop));
  return oopisGreaterThanOrEqualTo(objOop, oldSpaceStart);
}
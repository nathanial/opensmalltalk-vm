/* Extracted from interp.c:37993 (function keyOfEphemeron). */

/*	Answer the object the ephemeron guards. This is its first element. */

/* SpurMemoryManager>>#keyOfEphemeron: */

static sqInt keyOfEphemeron(sqInt objOop) {
  assert((isNonImmediate(objOop)) && (isObjEphemeron(objOop)));
  return fetchPointerofObject(0U, objOop);
}
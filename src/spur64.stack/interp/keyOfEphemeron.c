/* Extracted from interp.c:37993 (function keyOfEphemeron). */

/*	Answer the object the ephemeron guards. This is its first element. */

/* SpurMemoryManager>>#keyOfEphemeron: */

static NoDbgRegParms sqInt keyOfEphemeron(sqInt objOop) {
  assert((isNonImmediate(objOop)) && (isObjEphemeron(objOop)));
  return longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
}
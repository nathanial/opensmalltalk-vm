/* Extracted from interp.c:37231 (function isEmptyObjStack). */

/* SpurMemoryManager>>#isEmptyObjStack: */

static sqInt isEmptyObjStack(sqInt objStack) {
  if (objStack == nilObj) {
    return 1;
  }
  eassert(isValidObjStack(objStack));
  return (0 == (fetchPointerofObject(ObjStackTopx, objStack))) &&
         (0 == (fetchPointerofObject(ObjStackNextx, objStack)));
}
/* Extracted from interp.c:45600 (function sizeOfObjStack). */

/* SpurMemoryManager>>#sizeOfObjStack: */

static sqInt sizeOfObjStack(sqInt objStack) {
  sqInt objStackPage;
  sqInt total;

  if (objStack == nilObj) {
    return 0;
  }
  total = fetchPointerofObject(ObjStackTopx, objStack);
  objStackPage = objStack;
  while (1) {
    objStackPage = fetchPointerofObject(ObjStackNextx, objStackPage);
    if (!(objStackPage != 0))
      break;
    total += ObjStackLimit;
    assert((fetchPointerofObject(ObjStackTopx, objStackPage)) == ObjStackLimit);
  }
  return total;
}
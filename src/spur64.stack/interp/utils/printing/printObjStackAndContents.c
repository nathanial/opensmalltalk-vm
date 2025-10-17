/* Extracted from interp.c:44080 (function printObjStackAndContents). */

/* SpurMemoryManager>>#printObjStackAndContents: */

void printObjStackAndContents(sqInt objStack) {
  /* begin printObjStack:printContents: */
  if (objStack == nilObj) {
    print("nil");
    cr();
  } else {
    printObjStackPagemyIndexpageTypeprintContents(
        objStack,
        fetchPointerofObject(ObjStackMyx, objStack),
        ObjStackMyx, 1);
  }
}
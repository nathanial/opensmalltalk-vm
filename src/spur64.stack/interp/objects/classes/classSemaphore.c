/* Extracted from interp.c:33328 (function classSemaphore). */

/* SpurMemoryManager>>#classSemaphore */

sqInt classSemaphore(void) {
  return fetchPointerofObject(ClassSemaphore, specialObjectsOop);
}
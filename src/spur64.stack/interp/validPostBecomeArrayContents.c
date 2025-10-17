/* Extracted from interp.c:46838 (function validPostBecomeArrayContents). */

/*	This is a hook for the CoMemoryManagers to check for valid compiled
   code. It is a noop here. */

/* SpurMemoryManager>>#validPostBecomeArrayContents: */

static NoDbgRegParms sqInt validPostBecomeArrayContents(sqInt anArray) {
  return 1;
}
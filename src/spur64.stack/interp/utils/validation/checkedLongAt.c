/* Extracted from interp.c:32030 (function checkedLongAt). */

/*	Assumes zero-based array indexing. */

/* SpurMemoryManager>>#checkedLongAt: */

sqInt checkedLongAt(sqInt byteAddress) {
  if (!(isInMemory(byteAddress))) {
    warning("checkedLongAt bad address");

    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
  return longAt((void *)(byteAddress));
}
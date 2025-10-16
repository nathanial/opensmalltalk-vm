/* Extracted from interp.c:35050 (function fetchClassTagOfNonImm). */

/*	In Spur an object's classIndex is the tag in all method caches. */

/* SpurMemoryManager>>#fetchClassTagOfNonImm: */

static sqInt fetchClassTagOfNonImm(sqInt obj) {
  return (longAt((void *)(obj))) & (classIndexMask());
}
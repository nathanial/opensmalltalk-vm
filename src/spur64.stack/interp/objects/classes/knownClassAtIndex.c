/* Extracted from interp.c:38002 (function knownClassAtIndex). */

/* SpurMemoryManager>>#knownClassAtIndex: */

static sqInt knownClassAtIndex(sqInt classIndex) {
  assert(((classIndex >= 1) && (classIndex <= (classTablePageSize()))));
  return fetchPointerofObject(classIndex, classTableFirstPage);
}
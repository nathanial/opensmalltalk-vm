/* Extracted from interp.c:33281 (function classLargeNegativeInteger). */

/* SpurMemoryManager>>#classLargeNegativeInteger */

sqInt classLargeNegativeInteger(void) {
  /* begin knownClassAtIndex: */
  assert(((ClassLargeNegativeIntegerCompactIndex >= 1) &&
          (ClassLargeNegativeIntegerCompactIndex <= (classTablePageSize()))));
  return fetchPointerofObject(ClassLargeNegativeIntegerCompactIndex,
                              classTableFirstPage);
}
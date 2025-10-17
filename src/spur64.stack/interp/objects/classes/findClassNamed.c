/* Extracted from interp.c:35083 (function findClassNamed). */

/* SpurMemoryManager>>#findClassNamed: */

sqInt findClassNamed(char *aString) {
  sqInt classOrNil;
  sqInt i;
  sqInt ignored;
  sqInt j;
  sqInt page;
  sqInt toDoLimit;

  /* begin classTableEntriesDo: */
  for (i = 0; i < numClassTablePages; i += 1) {
    page = fetchPointerofObject(i, hiddenRootsObj);
    toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
    for (j = 0; j <= toDoLimit; j += 1) {
      classOrNil = fetchPointerofObject(j, page);
      if (classOrNil != nilObj) {
        ignored = ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
        if (classNameOfIs(classOrNil, aString)) {
          return classOrNil;
        }
      }
    }
  }
  return null;
}
/* Extracted from interp.c:43607 (function printInvalidClassTableEntries). */

/*	Print the objects in the classTable that have bad hashes. */
/*	useful for debugging */

/* SpurMemoryManager>>#printInvalidClassTableEntries */

void printInvalidClassTableEntries(void) {
  sqInt classOrNilSqInt;
  sqInt hash;
  sqInt i;
  sqInt index;
  sqInt j;
  sqInt page;
  sqInt toDoLimit;

  hash = 0;
  if (!(validClassTableRootPages())) {
    print("class table invalid; cannot print");
    cr();
    return;
  }

  /* begin classTableEntriesDo: */
  for (i = 0; i < numClassTablePages; i += 1) {
    page = fetchPointerofObject(i, hiddenRootsObj);
    toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
    for (j = 0; j <= toDoLimit; j += 1) {
      classOrNilSqInt = fetchPointerofObject(j, page);
      if (classOrNilSqInt != nilObj) {
        index = ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
        if (((!((longAt((void *)(classOrNilSqInt))) &
                ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) ||
            ((((hash = (long32At((void *)(classOrNilSqInt + 4))) &
                       (identityHashHalfWordMask()))) == 0) ||
             ((noCheckClassAtIndex(hash)) != classOrNilSqInt))) {
          print("entry ");
          printHex(index);
          print(" oop ");
          printHex(classOrNilSqInt);
          print(" hash ");
          printHex(hash);
          print(" => ");
          printHex(classAtIndex(hash));
          cr();
        }
      }
    }
  }
}
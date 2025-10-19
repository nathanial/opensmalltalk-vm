/* Extracted from interp.c:46724 (function validClassTableRootPages). */

/*	Answer if hiddenRootsObj is of the right size with the
        expected contents, and if numClassTablePages is correct. */

/* SpurMemoryManager>>#validClassTableRootPages */

static sqInt validClassTableRootPages(void) {
  sqInt i;
  sqInt obj;
  sqInt toDoLimit;

  if (!(((assert((classIndexOf(hiddenRootsObj)) >
                 (isForwardedObjectClassIndexPun())),
          numSlotsOf(hiddenRootsObj))) ==
        ((1U << (22 /* classIndexFieldWidth */ -
                 (classTableMajorIndexShift()))) +
         8 /* hiddenRootSlots */))) {
    return 0;
  }

  /* is it in range? */
  if (!((numClassTablePages > 1) &&
        (numClassTablePages <= (1U << (22 /* classIndexFieldWidth */ -
                                       (classTableMajorIndexShift())))))) {
    return 0;
  }

  /* are all pages the right size? */
  for (i = 0; i < numClassTablePages; i += 1) {
    obj = fetchPointerofObject(i, hiddenRootsObj);
    if (!((addressCouldBeObj(obj)) &&
          (((numSlotsOf(obj))) == (1U << (classTableMajorIndexShift()))))) {
      return 0;
    }
  }

  /* are all entries beyond numClassTablePages nil? */
  toDoLimit =
      (1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift()))) -
      1;
  for (i = numClassTablePages; i <= toDoLimit; i += 1) {
    if ((fetchPointerofObject(i, hiddenRootsObj)) != nilObj) {
      return 0;
    }
  }
  return 1;
}

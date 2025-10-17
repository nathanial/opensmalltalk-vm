/* Extracted from interp.c:35761 (function forwardersIn). */

/*	Answer if anObject is itself forwarded, or is a pointer object
   containing any references to forwarded objects.
 */

/* SpurMemoryManager>>#forwardersIn: */

static sqInt forwardersIn(sqInt anObject) {
  sqInt i;
  sqInt oop;
  sqInt toDoLimit;

  if ((!((longAt((void *)(anObject))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    return 1;
  }
  toDoLimit = (numPointerSlotsOf(anObject)) - 1;
  for (i = 0; i <= toDoLimit; i += 1) {
    oop = fetchPointerofObject(i, anObject);
    if (((!(oop & (tagMask())))) &&
        ((!((longAt((void *)(oop))) &
            ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
      return 1;
    }
  }
  return 0;
}
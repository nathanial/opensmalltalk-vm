/* Extracted from interp.c:33386 (function classTableRootSlots). */

/*	Answer the number of slots for class table pages in the hidden root
        object.
 */

/* SpurMemoryManager>>#classTableRootSlots */

static sqInt classTableRootSlots(void) {
  return 1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift()));
}
/* Extracted from interp.c:36996 (function instanceSizeOf). */

/*	Answer the number of slots in a class. For example the instanceSizeOf:
        ClassPoint is 2, for the x & y slots. The instance size of non-pointer
        classes is 0. */

/* SpurMemoryManager>>#instanceSizeOf: */

sqInt instanceSizeOf(sqInt classObj) {
  assert(addressCouldBeClassObj(classObj));
  return (((fetchPointerofObject(InstanceSpecificationIndex, classObj)) >>
           3)) &
         ((1U << (fixedFieldsFieldWidth())) - 1);
}
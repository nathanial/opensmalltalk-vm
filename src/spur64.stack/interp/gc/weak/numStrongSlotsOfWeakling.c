/* Extracted from interp.c:41328 (function numStrongSlotsOfWeakling). */

/*	Answer the number of strong pointer fields in the given weakling. */

/* SpurMemoryManager>>#numStrongSlotsOfWeakling: */

static sqInt numStrongSlotsOfWeakling(sqInt objOop) {
  sqInt objOopSqInt;

  assert((formatOf(objOop)) == (weakArrayFormat()));
  objOopSqInt = fetchClassOfNonImm(objOop);

  /* begin fixedFieldsOfClass: */
  return (((fetchPointerofObject(InstanceSpecificationIndex, objOopSqInt)) >>
           3)) &
         ((1U << (fixedFieldsFieldWidth())) - 1);
}
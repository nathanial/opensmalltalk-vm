/* Extracted from interp.c:35710 (function formatOfClass). */

/* SpurMemoryManager>>#formatOfClass: */

sqInt formatOfClass(sqInt classPointer) {
  return ((fetchPointerofObject(InstanceSpecificationIndex, classPointer)) >>
          3);
}
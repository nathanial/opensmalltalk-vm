/* Extracted from interp.c:37110 (function instSpecOfClass). */

/*	This field in a class's format inst var corresponds to the 5-bit format
        field stored in every object header
 */

/* SpurMemoryManager>>#instSpecOfClass: */

static sqInt instSpecOfClass(sqInt classPointer) {
  return (((usqInt)((((longAt((void *)((classPointer + BaseHeaderSize) +
                                       ((((usqInt)(InstanceSpecificationIndex)
                                          << (shiftForWord()))))))) >>
                      3)))) >>
          (fixedFieldsFieldWidth())) &
         (formatMask());
}
/* Extracted from interp.c:40697 (function
 * maybeMethodClassOfseemsToBeInstantiating). */

/*	Answers if the code is installed in a class instantiating objects with
   the format. Used in primitive generation to make a quick path based on where
   the method is installed. This method cannot be used as a guarantee as there
   can be false positive, it's just a heuristic. Tries to interpret the last
   literal of the method as a behavior (more than 3 fields, 3rd field a Smi). If
   it can be interpreted as a behavior, answers if instSpec matches the format,
   else answers false. */

/* SpurMemoryManager>>#maybeMethodClassOf:seemsToBeInstantiating: */

sqInt maybeMethodClassOfseemsToBeInstantiating(sqInt methodObj, sqInt format) {
  sqInt instSpec;
  sqInt literal;
  sqInt maybeClassObj;
  sqInt maybeFormat;
  sqInt offset;

  /* begin methodClassOf: */
  offset = (literalCountOf(methodObj)) - 1;
  literal = longAt(
      (void *)((methodObj + BaseHeaderSize) +
               ((((usqInt)((offset + LiteralStart)) << (shiftForWord()))))));
  if (/* isOopForwarded: */
      isOopForwarded(literal)) {
    literal = fixFollowedFieldofObjectwithInitialValue(offset + LiteralStart,
                                                       methodObj, literal);
  }
  maybeClassObj =
      ((literal != nilObj) &&
               (/* isPointers: */
                ((!(literal & (tagMask())))) &&
                (((byteAt((void *)(literal + (formatFieldByteOffset())))) &
                  (formatMask())) <= 5 /* lastPointerFormat */))
           ? (assert((numSlotsOf(literal)) > ValueIndex),
              followFieldofObject(ValueIndex, literal))
           : nilObj);
  if (!(((byteAt((void *)(maybeClassObj + (formatFieldByteOffset())))) &
         (formatMask())) <= 5 /* lastPointerFormat */)) {
    return 0;
  }
  if (!((numSlotsOfAny(maybeClassObj)) > InstanceSpecificationIndex)) {
    return 0;
  }
  maybeFormat = longAt(
      (void *)((maybeClassObj + BaseHeaderSize) +
               ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))));
  if (!((((maybeFormat) & 7) == 1))) {
    return 0;
  }
  instSpec = (((usqInt)(((maybeFormat >> 3)))) >> (fixedFieldsFieldWidth())) &
             (formatMask());
  return instSpec == format;
}
/* Extracted from interp.c:11588 (function positiveMachineIntegerValueOf). */

/*	Answer a value of an integer in address range, i.e up to the size of a
        machine word.
        The object may be either a positive SmallInteger or a
   LargePositiveInteger of size <= word size.
 */
/*	some important callers such as primitiveNewWithArg, so inline the common
        case
 */

/* InterpreterPrimitives>>#positiveMachineIntegerValueOf: */

usqIntptr_t positiveMachineIntegerValueOf(sqInt oop) {
  sqInt value;

  if ((((oop) & 7) == 1)) {
    value = (oop >> 3);
    if (value < 0) {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
      return null;
    }
    return value;
  }

  /* don't inline the rare case */
  return positiveMachineIntegerValueOfObj(oop);
}
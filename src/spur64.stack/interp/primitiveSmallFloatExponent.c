/* Extracted from interp.c:23189 (function primitiveSmallFloatExponent). */

/*	Answer the exponent part of this float. */

/* InterpreterPrimitives>>#primitiveSmallFloatExponent */

static void primitiveSmallFloatExponent(void) {
  sqInt aValue;
  usqInt exp;

  aValue =
      (((usqInt)(((/* begin exponentOfSmallFloat: */
                   assert(((longAt(stackPointer)) & (tagMask())) ==
                          (smallFloatTag())),
                   ((((usqInt)(longAt(stackPointer)))) <= 15
                        ? 0
                        : ((exp =
                                ((((usqInt)(longAt(stackPointer))))) >>
                                (((numTagBits()) + (smallFloatMantissaBits())) +
                                 1)),
                           (exp + (smallFloatExponentOffset())) - 0x3FE)))) -
                 1)
        << 3) |
       1);

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}
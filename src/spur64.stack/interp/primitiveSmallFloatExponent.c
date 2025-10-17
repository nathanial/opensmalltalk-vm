/* Extracted from interp.c:23189 (function primitiveSmallFloatExponent). */

/*	Answer the exponent part of this float. */

	/* InterpreterPrimitives>>#primitiveSmallFloatExponent */

static void
primitiveSmallFloatExponent(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aValue;
    usqInt exp;

	aValue = (((usqInt)(((/* begin exponentOfSmallFloat: */
	assert(((longAt(GIV(stackPointer))) & (tagMask())) == (smallFloatTag())),
((((usqInt)(longAt(GIV(stackPointer))))) <= 15
			? 0
			: ((exp = ((((usqInt)(longAt(GIV(stackPointer)))))) >> (((numTagBits()) + (smallFloatMantissaBits())) + 1)),
			(exp + (smallFloatExponentOffset())) - 0x3FE)))) - 1) << 3) | 1);

	/* begin stackTopPut: */
	longAtput(GIV(stackPointer),aValue);
}
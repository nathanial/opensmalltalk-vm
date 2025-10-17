/* Extracted from interp.c:22971 (function primitiveSmallFloatCosine). */

	/* InterpreterPrimitives>>#primitiveSmallFloatCosine */

static void
primitiveSmallFloatCosine(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aValue;
    usqLong bits;
    sqInt oop;
    double rcvr;

	oop = longAt(stackPointer);

	/* begin smallFloatValueOf: */
	/* begin smallFloatBitsOf: */
	assert(isImmediateFloat(oop));
	bits = ((((usqInt)oop))) >> (numTagBits());
	if (bits > 1) {
		bits += (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
	}

	/* a.k.a. ~= +/-0.0 */
	bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
	memcpy((&rcvr), (&bits), sizeof(rcvr));
	aValue = floatObjectOf(cos(rcvr));

	/* begin stackTopPut: */
	longAtput(stackPointer,aValue);
}
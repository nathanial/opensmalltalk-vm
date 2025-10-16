/* Extracted from interp.c:22943 (function primitiveSmallFloatArctan). */

static void
primitiveSmallFloatArctan(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aValue;
    usqLong bits;
    sqInt oop;
    double rcvr;

	oop = longAt(GIV(stackPointer));

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
	aValue = floatObjectOf(atan(rcvr));

	/* begin stackTopPut: */
	longAtput(GIV(stackPointer),aValue);
}
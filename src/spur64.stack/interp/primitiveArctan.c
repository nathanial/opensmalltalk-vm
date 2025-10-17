/* Extracted from interp.c:11819 (function primitiveArctan). */

/*	Computes arctan of float receiver; receiver *must* be a float instance. */

	/* InterpreterPrimitives>>#primitiveArctan */

static void
primitiveArctan(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aValue;
    usqLong bits;
    double doubleValue;
    sqInt rcvr;

	rcvr = longAt(stackPointer);

	/* begin noFailFloatValueOf: */
	assert(isFloatInstance(rcvr));
	if (rcvr & (tagMask())) {
		/* begin smallFloatValueOf: */
		/* begin smallFloatBitsOf: */
		assert(isImmediateFloat(rcvr));
		bits = ((((usqInt)rcvr))) >> (numTagBits());
		if (bits > 1) {
			bits += (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
		}

		/* a.k.a. ~= +/-0.0 */
		bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
		memcpy((&doubleValue), (&bits), sizeof(doubleValue));
	}
	else {
		fetchFloatAtinto(rcvr + BaseHeaderSize, doubleValue);
	}
	aValue = floatObjectOf(atan(doubleValue));

	/* begin stackTopPut: */
	longAtput(stackPointer,aValue);
}
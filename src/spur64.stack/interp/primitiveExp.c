/* Extracted from interp.c:15147 (function primitiveExp). */

/*	Computes E raised to the receiver power.
	Receiver *must* be a float instance. */

	/* InterpreterPrimitives>>#primitiveExp */

static void
primitiveExp(void)
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
	aValue = floatObjectOf(exp(doubleValue));

	/* begin stackTopPut: */
	longAtput(stackPointer,aValue);
}
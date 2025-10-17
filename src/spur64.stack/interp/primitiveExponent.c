/* Extracted from interp.c:15185 (function primitiveExponent). */

/*	Exponent part of float receiver; receiver *must* be a float instance. */

	/* InterpreterPrimitives>>#primitiveExponent */

static void
primitiveExponent(void)
{
    usqLong bits;
    double doubleValue;
    int pwr;
    sqInt rcvr;

	pwr = 0;
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

	/* rcvr = frac * 2^pwr, where frac is in [0.5..1.0) */
	frexp(doubleValue, (&pwr));

	/* stackTopPut: */
	longAtput(stackPointer,(((usqInt)(pwr - 1) << 3) | 1));
}
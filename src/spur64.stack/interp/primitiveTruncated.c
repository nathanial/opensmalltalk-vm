/* Extracted from interp.c:25373 (function primitiveTruncated). */

static void
primitiveTruncated(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqLong bits;
    double doubleValue;
    sqInt rcvr;
    double trunc;

	rcvr = longAt(GIV(stackPointer));

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
	modf(doubleValue, (&trunc));
	if (((trunc >= (((double) (MinSmallInteger) ))) && (trunc <= (((double) (MaxSmallInteger) ))))) {
		/* stackTopPut: */
		longAtput(GIV(stackPointer),(((usqInt)(((sqInt)trunc)) << 3) | 1));
		return;
	}

	/* make Nicolas Cellier's otherwise recursive BoxedFloat64>>truncated nice 2/7/2025 07:34 work on 32-bits. */

	/* a.k.a. trunc abs <= Float maxExactInteger asFloat */

	/* begin primitiveFail */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
}
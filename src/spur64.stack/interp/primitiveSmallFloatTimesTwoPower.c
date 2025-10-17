/* Extracted from interp.c:23939 (function primitiveSmallFloatTimesTwoPower). */

/*	Multiply the receiver by the power of the argument. */

	/* InterpreterPrimitives>>#primitiveSmallFloatTimesTwoPower */

static void
primitiveSmallFloatTimesTwoPower(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    sqInt rcvr;
    sqInt result;
    char *sp;
    sqInt twiceMaxExponent;

	arg = longAt(stackPointer);
	if (!((((arg) & 7) == 1))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	rcvr = longAt(stackPointer + (1 * BytesPerWord));

	/* N.B. SmallFloats are finite.  NaN and Infinity overflow into boxed floats.
	   This is doing range checking work that is done in ldexp, but we include
	   it explicitly to exemplify bit manipulation of SmallFloats. */

	/* begin isSmallFloatZero: */
	assert(isImmediateFloat(rcvr));
	if ((((usqInt)rcvr)) <= ((1U << (numTagBits())) + (smallFloatTag()))) {
		result = rcvr;
	}
	else {
		arg = (arg >> 3);
		twiceMaxExponent = 0x1000;
		if (arg < (-twiceMaxExponent)) {
			/* begin mapSignedSmallFloatToSignedSmallFloatZero: */
			assert(isImmediateFloat(rcvr));
			result = rcvr & ((1U << (numTagBits())) + (tagMask()));
		}
		else {
			if (arg > twiceMaxExponent) {
				arg = twiceMaxExponent;
			}
			result = floatObjectOf(ldexp(smallFloatValueOf(rcvr), ((int) arg)));
		}
	}

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer + (1 * BytesPerWord)),result);
	stackPointer = sp;
}
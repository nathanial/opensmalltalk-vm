/* Extracted from interp.c:23633 (function primitiveSmallFloatMultiply). */

	/* InterpreterPrimitives>>#primitiveSmallFloatMultiply */

static void
primitiveSmallFloatMultiply(void)
{
    double arg;
    usqLong bits;
    sqInt floatOrIntOop;
    sqInt oop;
    double rcvr;
    double result;
    char *sp;
    sqInt tagBits;
    double value;

	oop = longAt(stackPointer + (1 * BytesPerWord));

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
	floatOrIntOop = longAt(stackPointer);

	/* begin loadFloatOrIntFrom: */
	if ((tagBits = floatOrIntOop & (tagMask()))) {
		if (tagBits == (smallFloatTag())) {
			/* begin smallFloatValueOf: */
			/* begin smallFloatBitsOf: */
			assert(isImmediateFloat(floatOrIntOop));
			bits = ((((usqInt)floatOrIntOop))) >> (numTagBits());
			if (bits > 1) {
				bits += (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
			}

			/* a.k.a. ~= +/-0.0 */
			bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
			memcpy((&value), (&bits), sizeof(value));
			arg = value;
			goto l1;
		}
		if ((primitiveDoMixedArithmetic)
		 && (tagBits == (smallIntegerTag()))) {
			arg = ((double) ((floatOrIntOop >> 3)) );
			goto l1;
		}
	}
	else {
		if (((longAt((void *)(floatOrIntOop))) & (classIndexMask())) == ClassFloatCompactIndex) {
			fetchFloatAtinto(floatOrIntOop + BaseHeaderSize, result);
			arg = result;
			goto l1;
		}
	}

	/* begin primitiveFail */
	if (!primFailCode) {
		primFailCode = 1;
	}
	arg = 0.0;
	/* end loadFloatOrIntFrom: */
l1:
	if (!primFailCode) {
		/* begin pop:thenPushFloat: */
		longAtput((sp = stackPointer + (1 * BytesPerWord)),floatObjectOf(rcvr * arg));
		stackPointer = sp;
	}
}
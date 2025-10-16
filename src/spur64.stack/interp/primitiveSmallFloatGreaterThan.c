/* Extracted from interp.c:23329 (function primitiveSmallFloatGreaterThan). */

static void
primitiveSmallFloatGreaterThan(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    double arg;
    usqLong bits;
    sqInt floatOrIntOop;
    sqInt intArg;
    sqInt oop;
    double rcvr;
    double result;
    char *sp;
    sqInt tagBits;
    double value;

	oop = longAt(GIV(stackPointer) + (1 * BytesPerWord));

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
	floatOrIntOop = longAt(GIV(stackPointer));

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
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	arg = 0.0;
	/* end loadFloatOrIntFrom: */
l1:
	if (!GIV(primFailCode)) {
		if ((rcvr == arg)
		 && (((((longAt(GIV(stackPointer)))) & 7) == 1))) {
			intArg = ((longAt(GIV(stackPointer))) >> 3);

			/* begin pop:thenPushBool: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
				((((sqInt)rcvr)) > intArg
					? GIV(trueObj)
					: GIV(falseObj)));
			GIV(stackPointer) = sp;
		}
		else {
			/* begin pop:thenPushBool: */
			longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),/* booleanObjectOf: */
				(rcvr > arg
					? GIV(trueObj)
					: GIV(falseObj)));
			GIV(stackPointer) = sp;
		}
	}
}
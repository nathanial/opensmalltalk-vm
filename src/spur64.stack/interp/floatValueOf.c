/* Extracted from interp.c:27022 (function floatValueOf). */

/*	Answer the C double precision floating point value of the argument,
	or fail if it is not a Float, and answer 0.
	Note: May be called by translated primitive code. */

	/* Spur64BitMemoryManager>>#floatValueOf: */

double
floatValueOf(sqInt oop)
{
    usqLong bits;
    double result;
    sqInt tagBits;
    double value;

	if ((tagBits = oop & (tagMask()))) {
		if (tagBits == (smallFloatTag())) {
			/* begin smallFloatValueOf: */
			/* begin smallFloatBitsOf: */
			assert(isImmediateFloat(oop));
			bits = ((((usqInt)oop))) >> (numTagBits());
			if (bits > 1) {
				bits += (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
			}

			/* a.k.a. ~= +/-0.0 */
			bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
			memcpy((&value), (&bits), sizeof(value));
			return value;
		}
	}
	else {
		if (((longAt((void *)(oop))) & (classIndexMask())) == ClassFloatCompactIndex) {
			fetchFloatAtinto(oop + BaseHeaderSize, result);
			return result;
		}
	}

	/* begin primitiveFail */
	if (!primFailCode) {
		primFailCode = 1;
	}
	return 0.0;
}
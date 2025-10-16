/* Extracted from interp.c:27463 (function loadFloatOrIntFrom). */

static NoDbgRegParms double
loadFloatOrIntFrom(sqInt floatOrIntOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqLong bits;
    double result;
    sqInt tagBits;
    double value;

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
			return value;
		}
		if ((primitiveDoMixedArithmetic)
		 && (tagBits == (smallIntegerTag()))) {
			return ((double) ((floatOrIntOop >> 3)) );
		}
	}
	else {
		if (((longAt((void *)(floatOrIntOop))) & (classIndexMask())) == ClassFloatCompactIndex) {
			fetchFloatAtinto(floatOrIntOop + BaseHeaderSize, result);
			return result;
		}
	}

	/* begin primitiveFail */
	if (!GIV(primFailCode)) {
		GIV(primFailCode) = 1;
	}
	return 0.0;
}
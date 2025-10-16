/* Extracted from interp.c:27523 (function noFailFloatValueOf). */

double
noFailFloatValueOf(sqInt aFloatOop)
{
    usqLong bits;
    double result;

	assert(isFloatInstance(aFloatOop));
	if (aFloatOop & (tagMask())) {
		/* begin smallFloatValueOf: */
		/* begin smallFloatBitsOf: */
		assert(isImmediateFloat(aFloatOop));
		bits = ((((usqInt)aFloatOop))) >> (numTagBits());
		if (bits > 1) {
			bits += (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
		}

		/* a.k.a. ~= +/-0.0 */
		bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
		memcpy((&result), (&bits), sizeof(result));
	}
	else {
		fetchFloatAtinto(aFloatOop + BaseHeaderSize, result);
	}
	return result;
}
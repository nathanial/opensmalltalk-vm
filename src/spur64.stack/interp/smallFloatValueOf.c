/* Extracted from interp.c:27632 (function smallFloatValueOf). */

static NoDbgRegParms double
smallFloatValueOf(sqInt oop)
{
    usqLong bits;
    double value;

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
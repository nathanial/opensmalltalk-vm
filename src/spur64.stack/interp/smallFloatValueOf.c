/* Extracted from interp.c:27632 (function smallFloatValueOf). */

/*	Answer the C double precision floating point value of the argument, a
	SmallFloat. See section 61-bit Immediate Floats in the SpurMemoryManager
	class comment.
	msb lsb 
	Decode:				[8expsubset][52mantissa][1s][3tags] 
	shift away tags:		[ 000 ][8expsubset][52mantissa][1s] 
	add exponent offset:	[ 11 exponent ][52mantissa][1s] 
	rot sign:				[1s][ 11 exponent ][52mantissa] */

	/* Spur64BitMemoryManager>>#smallFloatValueOf: */

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
/* Extracted from interp.c:15785 (function primitiveFloatAt). */

static void
primitiveFloatAt(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqLong bits;
    sqInt fieldIndex;
    sqInt index;
    unsigned int integerValue;
    sqInt rcvr;
    usqInt result;
    char *sp;

	rcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	index = longAt(GIV(stackPointer));
	if (index == ConstOne) {
		fieldIndex = (VMBIGENDIAN
					? 0
					: 1);

		/* begin fetchLong32:ofFloatObject: */
		if (!(((rcvr & (smallFloatTag())) != 0))) {
			integerValue = ((unsigned int) (long32At((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(fieldIndex) << 2)))))));
			goto l1;
		}

		/* begin smallFloatBitsOf: */
		assert(isImmediateFloat(rcvr));
		bits = ((((usqInt)rcvr))) >> (numTagBits());
		if (bits > 1) {
			bits += (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
		}

		/* a.k.a. ~= +/-0.0 */
		bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
		integerValue = ((unsigned int) ((fieldIndex
		? (bits) >> 32
		: bits & 0xFFFFFFFFU)));
		/* end fetchLong32:ofFloatObject: */
l1:

		/* begin positive32BitIntegerFor: */
		result = ((((((usqInt)integerValue)) & 0xFFFFFFFFU) << 3) | 1);
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),result);
		GIV(stackPointer) = sp;
		return;
	}
	if (index == ConstTwo) {
		fieldIndex = (VMBIGENDIAN
					? 1
					: 0);

		/* begin fetchLong32:ofFloatObject: */
		if (!(((rcvr & (smallFloatTag())) != 0))) {
			integerValue = ((unsigned int) (long32At((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(fieldIndex) << 2)))))));
			goto l2;
		}

		/* begin smallFloatBitsOf: */
		assert(isImmediateFloat(rcvr));
		bits = ((((usqInt)rcvr))) >> (numTagBits());
		if (bits > 1) {
			bits += (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
		}

		/* a.k.a. ~= +/-0.0 */
		bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
		integerValue = ((unsigned int) ((fieldIndex
		? (bits) >> 32
		: bits & 0xFFFFFFFFU)));
		/* end fetchLong32:ofFloatObject: */
l2:

		/* begin positive32BitIntegerFor: */
		result = ((((((usqInt)integerValue)) & 0xFFFFFFFFU) << 3) | 1);
		longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),result);
		GIV(stackPointer) = sp;
		return;
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = ((((index) & 7) == 1)
				? PrimErrBadIndex
				: PrimErrBadArgument);
}
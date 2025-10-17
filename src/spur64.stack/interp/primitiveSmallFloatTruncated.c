/* Extracted from interp.c:23987 (function primitiveSmallFloatTruncated). */

	/* InterpreterPrimitives>>#primitiveSmallFloatTruncated */

static void
primitiveSmallFloatTruncated(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqLong bits;
    sqInt oop;
    double rcvr;
    double trunc;

	oop = longAt(stackPointer);

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
	modf(rcvr, (&trunc));
	if ((((trunc >= (((double) (MinSmallInteger) ))) && (trunc <= (((double) (MaxSmallInteger) )))))
	 && ((((((usqInt)((((sqInt)trunc)))) >> 60) + 1) & 15) <= 1)) {
		/* stackTopPut: */
		longAtput(stackPointer,(((usqInt)(((sqInt)trunc)) << 3) | 1));
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
}
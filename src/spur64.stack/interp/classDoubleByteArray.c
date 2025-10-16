/* Extracted from interp.c:33084 (function classDoubleByteArray). */

sqInt
classDoubleByteArray(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classOop;
    sqInt hash;

	/* begin accessIntegerClass:withValidationFlag: */
	hash = 0;
	classOop = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassDoubleByteArray) << (shiftForWord()))))));
	if (((GIV(validatedIntegerClassFlags) & ValidatedClassDoubleByteArrayFlag) != 0)) {
		return classOop;
	}
	if ((((classOop & (tagMask())) != 0))
	 || ((((hash = (long32At((void *)(classOop + 4))) & (identityHashHalfWordMask()))) == 0)
	 || ((classAtIndex(hash)) != classOop))) {
		return GIV(nilObj);
	}
	GIV(validatedIntegerClassFlags) = GIV(validatedIntegerClassFlags) | ValidatedClassDoubleByteArrayFlag;
	return classOop;
}
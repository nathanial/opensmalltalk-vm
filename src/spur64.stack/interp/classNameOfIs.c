/* Extracted from interp.c:51435 (function classNameOfIs). */

/*	Check if aClass's name is className */

	/* StackInterpreter>>#classNameOf:Is: */

static NoDbgRegParms sqInt
classNameOfIs(sqInt aClass, char *className)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt fmt;
    sqInt i;
    sqInt length;
    sqInt name;
    usqInt numSlots;
    char *srcName;

	if (((/* begin numSlotsOf: */
		assert((classIndexOf(aClass)) > (isForwardedObjectClassIndexPun())),
	(((numSlots = byteAt((void *)(aClass + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(aClass - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots))) <= GIV(classNameIndex)) {
		return 0;
	}
	name = longAt((void *)((aClass + BaseHeaderSize) + ((((usqInt)(GIV(classNameIndex)) << (shiftForWord()))))));
	if (!(/* isBytes: */
			((!(name & (tagMask()))))
		 && (((byteAt((void *)(name + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))) {
		return 0;
	}

	/* begin numBytesOfBytes: */
	fmt = (byteAt((void *)(name + (formatFieldByteOffset())))) & (formatMask());
	assert(fmt >= (firstByteFormat()));
	length = ((((/* begin numSlotsOf: */
	assert((classIndexOf(name)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(name + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(name - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);
	srcName = ((char *) (arrayValueOf(name)));
	for (i = 0; i < length; i += 1) {
		if (!((srcName[i]) == (className[i]))) {
			return 0;
		}
	}

	/* Check if className really ends at this point */
	return (className[length]) == 0;
}
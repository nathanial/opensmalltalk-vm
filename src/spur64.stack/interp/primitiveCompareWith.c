/* Extracted from interp.c:14262 (function primitiveCompareWith). */

EXPORT(void)
primitiveCompareWith(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt c1;
    sqInt c2;
    sqInt fmt;
    sqInt i;
    sqInt min;
    usqInt numSlots;
    sqInt order;
    sqInt result;
    char *sp;
    sqInt string1;
    sqInt string2;
    sqInt strLength1;
    sqInt strLength2;

	order = 0;
	if (!(GIV(argumentCount) == 1)) {
		if (GIV(argumentCount) != 2) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadNumArgs;
			return;
		}
		order = longAt(GIV(stackPointer));
		if (!((/* isBytes: */
				((!(order & (tagMask()))))
			 && (((byteAt((void *)(order + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))
			 && ((numBytesOfBytes(order)) == 0x100))) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
			return;
		}
	}
	string1 = longAt(GIV(stackPointer) + (GIV(argumentCount) * BytesPerWord));

	/* 2 - check their types - all parameters are ByteObject */
	string2 = longAt(GIV(stackPointer) + ((GIV(argumentCount) - 1) * BytesPerWord));
	if (!((/* isBytes: */
			((!(string1 & (tagMask()))))
		 && (((byteAt((void *)(string1 + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))
		 && (/* isBytes: */
			((!(string2 & (tagMask()))))
		 && (((byteAt((void *)(string2 + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat()))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}

	/* 3 - compare the strings */

	/* begin numBytesOfBytes: */
	fmt = (byteAt((void *)(string1 + (formatFieldByteOffset())))) & (formatMask());
	assert(fmt >= (firstByteFormat()));
	strLength1 = ((((/* begin numSlotsOf: */
	assert((classIndexOf(string1)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(string1 + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(string1 - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);

	/* begin numBytesOfBytes: */
	fmt = (byteAt((void *)(string2 + (formatFieldByteOffset())))) & (formatMask());
	assert(fmt >= (firstByteFormat()));
	strLength2 = ((((/* begin numSlotsOf: */
	assert((classIndexOf(string2)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(string2 + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(string2 - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);
	if (order) {
		/* begin rawCompare:length:with:length:accessBlock: */
		min = ((strLength1 < strLength2) ? strLength1 : strLength2);
		for (i = 0; i < min; i += 1) {
			c1 = byteAt((void *)((order + BaseHeaderSize) + ((byteAt((void *)((string1 + BaseHeaderSize) + i))) + 1)));
			c2 = byteAt((void *)((order + BaseHeaderSize) + ((byteAt((void *)((string2 + BaseHeaderSize) + i))) + 1)));
			if (!(c1 == c2)) {
				result = c1 - c2;
				goto l2;
			}
		}
		result = strLength1 - strLength2;
		/* end rawCompare:length:with:length:accessBlock: */
l2:;
	}
	else {
		/* begin rawCompare:length:with:length:accessBlock: */
		min = ((strLength1 < strLength2) ? strLength1 : strLength2);
		for (i = 0; i < min; i += 1) {
			c1 = byteAt((void *)((string1 + BaseHeaderSize) + i));
			c2 = byteAt((void *)((string2 + BaseHeaderSize) + i));
			if (!(c1 == c2)) {
				result = c1 - c2;
				goto l1;
			}
		}
		result = strLength1 - strLength2;
		/* end rawCompare:length:with:length:accessBlock: */
l1:;
	}

	/* begin methodReturnInteger: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)result << 3) | 1));
	GIV(stackPointer) = sp;
}
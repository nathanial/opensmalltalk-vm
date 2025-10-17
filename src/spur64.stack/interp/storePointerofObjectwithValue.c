/* Extracted from interp.c:46067 (function storePointerofObjectwithValue). */

/*	Note must check here for stores of young objects into old ones. */
/*	See SistaCogit */

	/* SpurMemoryManager>>#storePointer:ofObject:withValue: */

sqInt
storePointerofObjectwithValue(sqInt fieldIndex, sqInt objOop, sqInt valuePointer)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(validStorePointerArgs(fieldIndex, objOop, valuePointer));

	/* begin isOldObject: */
	assert(isNonImmediate(objOop));
	if (oopisGreaterThanOrEqualTo(objOop, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(valuePointer & (tagMask()))))
		 && (oopisLessThan(valuePointer, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(objOop);
			}
		}
	}

	/* most stores into young objects */
	return longAtput((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),valuePointer);
}
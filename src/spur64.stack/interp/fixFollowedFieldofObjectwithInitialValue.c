/* Extracted from interp.c:35474 (function fixFollowedFieldofObjectwithInitialValue). */

/*	Private helper for followField:ofObject: to avoid code duplication for
	rare case.
 */

	/* SpurMemoryManager>>#fixFollowedField:ofObject:withInitialValue: */

static NoDbgRegParms NeverInline sqInt
fixFollowedFieldofObjectwithInitialValue(sqInt fieldIndex, sqInt anObject, sqInt initialValue)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt objOop;

	assert(isOopForwarded(initialValue));

	/* inlined followForwarded: for speed (one less test) */
	objOop = initialValue;
	do {
		objOop = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
	} while(/* isOopForwarded: */
			((!(objOop & (tagMask()))))
		 && ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))));

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(fieldIndex, anObject, objOop));
	assert(isNonImmediate(anObject));
	if (oopisGreaterThanOrEqualTo(anObject, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(objOop & (tagMask()))))
		 && (oopisLessThan(objOop, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(anObject + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(anObject);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((anObject + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),objOop);
	return objOop;
}
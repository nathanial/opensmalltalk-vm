/* Extracted from interp.c:64815 (function storeIntegerofObjectwithValue). */

/*	Note: May be called by translated primitive code. */

	/* StackInterpreter>>#storeInteger:ofObject:withValue: */

sqInt
storeIntegerofObjectwithValue(sqInt fieldIndex, sqInt objectPointer, sqInt integerValue)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if ((((((usqInt)(integerValue)) >> 60) + 1) & 15) <= 1) {
		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(objectPointer))
		 && (!(isForwarded(objectPointer))));
		assert(validStorePointerUncheckedArgs(fieldIndex, objectPointer, (((usqInt)integerValue << 3) | 1)));
		longAtput((void *)((objectPointer + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),(((usqInt)integerValue << 3) | 1));
	}
	else {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
	return null;
}
/* Extracted from interp.c:13472 (function primitiveClipboardText). */

static void
primitiveClipboardText(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt fmt;
    usqInt numSlots;
    sqInt s;
    char *sp;
    sqInt sz;

	if (GIV(argumentCount) == 1) {
		s = longAt(GIV(stackPointer));
		if (!(/* isBytes: */
				((!(s & (tagMask()))))
			 && (((byteAt((void *)(s + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return;
		}
		if (!GIV(primFailCode)) {
			/* begin numBytesOfBytes: */
			fmt = (byteAt((void *)(s + (formatFieldByteOffset())))) & (formatMask());
			assert(fmt >= (firstByteFormat()));
			sz = ((((/* begin numSlotsOf: */
	assert((classIndexOf(s)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(s + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(s - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);
			clipboardWriteFromAt(sz, s + BaseHeaderSize, 0);

			/* begin pop: */
			GIV(stackPointer) += 1 * BytesPerWord;
		}
	}
	else {
		sz = clipboardSize();

		/* begin allocateBytes:classIndex: */
		assert(addressCouldBeClassObj(classAtIndex(ClassByteStringCompactIndex)));
		assert((instSpecOfClass(classAtIndex(ClassByteStringCompactIndex))) == (firstByteFormat()));
		s = noInlineAllocateSlotsformatclassIndex(((sz + BytesPerOop) - 1) / BytesPerOop, byteFormatForNumBytes(sz), ClassByteStringCompactIndex);
		if (!s) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return;
		}
		clipboardReadIntoAt(sz, s + BaseHeaderSize, 0);

		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer)),s);
		GIV(stackPointer) = sp;
	}
}
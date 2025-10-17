/* Extracted from interp.c:13472 (function primitiveClipboardText). */

/*	When called with a single string argument, post the string to 
	the clipboard. When called with zero arguments, return a 
	string containing the current clipboard contents. */

	/* InterpreterPrimitives>>#primitiveClipboardText */

static void
primitiveClipboardText(void)
{
    sqInt fmt;
    usqInt numSlots;
    sqInt s;
    char *sp;
    sqInt sz;

	if (argumentCount == 1) {
		s = longAt(stackPointer);
		if (!(/* isBytes: */
				((!(s & (tagMask()))))
			 && (((byteAt((void *)(s + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
			return;
		}
		if (!primFailCode) {
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
			stackPointer += 1 * BytesPerWord;
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
			if (!primFailCode) {
				primFailCode = 1;
			}
			return;
		}
		clipboardReadIntoAt(sz, s + BaseHeaderSize, 0);

		/* begin pop:thenPush: */
		longAtput((sp = stackPointer),s);
		stackPointer = sp;
	}
}
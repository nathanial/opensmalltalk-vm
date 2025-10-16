/* Extracted from interp.c:22429 (function primitiveShortAtPut). */

static void
primitiveShortAtPut(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt index;
    sqInt rcvr;
    char *sp;
    sqInt value;

	value = longAt(GIV(stackPointer));
	index = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (!(((((value) & 7) == 1))
		 && (((((index) & 7) == 1))
		 && (((value = (value >> 3)),
		(value >= -32768)
			 && (value <= 0x7FFF)))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	rcvr = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	if (!(/* isWordsOrBytes: */
			((!(rcvr & (tagMask()))))
		 && (isWordsOrBytesNonImm(rcvr)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrInappropriate;
		return;
	}
	if (
#  if IMMUTABILITY
		((((usqInt)((byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0
#  else
		0
#  endif
		) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrNoModification;
		return;
	}
	index = (index >> 3);
	if (!((index >= 1)
		 && (index <= (((usqInt)((numBytesOf(rcvr)))) >> 1)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}

	/* storeShort16:ofObject:withValue: */
	shortAtput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)((index - 1)) << 1)))),value);

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (2 * BytesPerWord)),(((usqInt)value << 3) | 1));
	GIV(stackPointer) = sp;
}
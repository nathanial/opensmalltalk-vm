/* Extracted from interp.c:22384 (function primitiveShortAt). */

static void
primitiveShortAt(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt index;
    sqInt rcvr;
    char *sp;
    sqInt value;

	index = longAt(GIV(stackPointer));
	if (!((((index) & 7) == 1))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	rcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (!(/* isWordsOrBytes: */
			((!(rcvr & (tagMask()))))
		 && (isWordsOrBytesNonImm(rcvr)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrInappropriate;
		return;
	}
	index = (index >> 3);
	if (!((index >= 1)
		 && (index <= (((usqInt)((numBytesOf(rcvr)))) >> 1)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}
	value = shortAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)((index - 1)) << 1)))));

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer) + (1 * BytesPerWord)),(((usqInt)value << 3) | 1));
	GIV(stackPointer) = sp;
}
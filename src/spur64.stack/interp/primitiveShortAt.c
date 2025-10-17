/* Extracted from interp.c:22384 (function primitiveShortAt). */

/*	Treat the receiver, which can be indexible by either bytes or words, as
	an array of signed 16-bit values. Answer the contents of the given index.
	Note that the index specifies the i-th 16-bit entry, not the i-th byte or
	word.  */

	/* InterpreterPrimitives>>#primitiveShortAt */

static void
primitiveShortAt(void)
{
    sqInt index;
    sqInt rcvr;
    char *sp;
    sqInt value;

	index = longAt(stackPointer);
	if (!((((index) & 7) == 1))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	rcvr = longAt(stackPointer + (1 * BytesPerWord));
	if (!(/* isWordsOrBytes: */
			((!(rcvr & (tagMask()))))
		 && (isWordsOrBytesNonImm(rcvr)))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrInappropriate;
		return;
	}
	index = (index >> 3);
	if (!((index >= 1)
		 && (index <= (((usqInt)((numBytesOf(rcvr)))) >> 1)))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadIndex;
		return;
	}
	value = shortAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)((index - 1)) << 1)))));

	/* begin pop:thenPushInteger: */
	longAtput((sp = stackPointer + (1 * BytesPerWord)),(((usqInt)value << 3) | 1));
	stackPointer = sp;
}
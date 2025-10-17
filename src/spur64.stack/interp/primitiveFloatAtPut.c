/* Extracted from interp.c:15875 (function primitiveFloatAtPut). */

/*	Provide platform-independent access to 32-bit words comprising
	a Float. Map index 1 onto the most significant word and index 2
	onto the least significant word. */

	/* InterpreterPrimitives>>#primitiveFloatAtPut */

static void
primitiveFloatAtPut(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt index;
    sqInt oopToStore;
    sqInt rcvr;
    char *sp;
    usqInt valueToStore;

	oopToStore = longAt(stackPointer);
	valueToStore = positive32BitValueOf(oopToStore);
	if (primFailCode) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	rcvr = longAt(stackPointer + (2 * BytesPerWord));
	index = longAt(stackPointer + (1 * BytesPerWord));
	if (((rcvr & (smallFloatTag())) != 0)) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadReceiver;
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
		primFailCode = PrimErrNoModification;
		return;
	}
	if (index == ConstOne) {
		/* storeLong32:ofObject:withValue: */
		long32Atput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(((VMBIGENDIAN
		? 0
		: 1))) << 2)))),valueToStore);

		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (2 * BytesPerWord)),oopToStore);
		stackPointer = sp;
		return;
	}
	if (index == ConstTwo) {
		/* storeLong32:ofObject:withValue: */
		long32Atput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(((VMBIGENDIAN
		? 1
		: 0))) << 2)))),valueToStore);

		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (2 * BytesPerWord)),oopToStore);
		stackPointer = sp;
		return;
	}

	/* primitiveFailFor: */
	primFailCode = ((((index) & 7) == 1)
				? PrimErrBadIndex
				: PrimErrBadArgument);
}
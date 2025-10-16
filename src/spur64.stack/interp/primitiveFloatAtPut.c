/* Extracted from interp.c:15875 (function primitiveFloatAtPut). */

static void
primitiveFloatAtPut(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt index;
    sqInt oopToStore;
    sqInt rcvr;
    char *sp;
    usqInt valueToStore;

	oopToStore = longAt(GIV(stackPointer));
	valueToStore = positive32BitValueOf(oopToStore);
	if (GIV(primFailCode)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	rcvr = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	index = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (((rcvr & (smallFloatTag())) != 0)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
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
	if (index == ConstOne) {
		/* storeLong32:ofObject:withValue: */
		long32Atput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(((VMBIGENDIAN
		? 0
		: 1))) << 2)))),valueToStore);

		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (2 * BytesPerWord)),oopToStore);
		GIV(stackPointer) = sp;
		return;
	}
	if (index == ConstTwo) {
		/* storeLong32:ofObject:withValue: */
		long32Atput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(((VMBIGENDIAN
		? 1
		: 0))) << 2)))),valueToStore);

		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (2 * BytesPerWord)),oopToStore);
		GIV(stackPointer) = sp;
		return;
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = ((((index) & 7) == 1)
				? PrimErrBadIndex
				: PrimErrBadArgument);
}
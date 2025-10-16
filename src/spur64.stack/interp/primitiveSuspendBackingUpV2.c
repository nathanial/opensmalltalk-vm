/* Extracted from interp.c:70184 (function primitiveSuspendBackingUpV2). */

EXPORT(void)
primitiveSuspendBackingUpV2(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt myContext;
    sqInt myList;
    usqInt numSlots;
    sqInt ok;
    sqInt process;

	process = longAt(GIV(stackPointer));
	if (process == (activeProcess())) {
		/* stackTopPut: */
		longAtput(GIV(stackPointer),GIV(nilObj));

		/* transferToHighestPriorityProcessFrom: */
		transferTofrom(wakeHighestPriority(), CSSuspend);
		return;
	}
	myList = longAt((void *)((process + BaseHeaderSize) + ((((usqInt)(MyListIndex) << (shiftForWord()))))));
	myContext = longAt((void *)((process + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))));
	if (!((/* isPointers: */
			((!(myList & (tagMask()))))
		 && (((byteAt((void *)(myList + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
		 && ((((/* begin numSlotsOf: */
			assert((classIndexOf(myList)) > (isForwardedObjectClassIndexPun())),
		(((numSlots = byteAt((void *)(myList + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(myList - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots))) > LastLinkIndex)
		 && ((((longAt((void *)(myContext))) & (classIndexMask())) == ClassMethodContextCompactIndex)
		 && ((/* begin isResumableContext: */
			assert(isContext(myContext)),
		((((longAt((void *)((myContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord())))))))) & 7) == 1))))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		return;
	}
	ok = removeProcessfromList(process, myList);
	if (!ok) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrOperationFailed;
		return;
	}
	if (((longAt((void *)(myList))) & (classIndexMask())) != GIV(classLinkedListClassTag)) {
		backupContexttoBlockingSendTo(myContext, myList);

		/* stackTopPut: */
		longAtput(GIV(stackPointer),GIV(nilObj));
	}
	else {
		/* stackTopPut: */
		longAtput(GIV(stackPointer),myList);
	}
}
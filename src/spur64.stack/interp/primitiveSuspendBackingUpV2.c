/* Extracted from interp.c:70184 (function primitiveSuspendBackingUpV2). */

/*	Primitive #578. Suspend the receiver, aProcess, such that it can be
	executed again
	by sending #resume. If the given process is not the active process, take
	it off
	its corresponding list. If the list was not its run queue assume it was on
	some condition variable (Semaphore, Mutex) and back up its pc to the send
	that invoked the wait state the process entered. Hence when the process
	resumes it will reenter the wait state. Answer the list the receiver was
	previously on iff
	it was not active and not blocked, otherwise answer nil.
	c.f. primitiveSuspend,#88 & primitiveSuspendBackingUpV1,#568,
	which always answer the list the process was on, even if blocked. */

	/* StackInterpreterPrimitives>>#primitiveSuspendBackingUpV2 */

EXPORT(void)
primitiveSuspendBackingUpV2(void)
{
    sqInt myContext;
    sqInt myList;
    usqInt numSlots;
    sqInt ok;
    sqInt process;

	process = longAt(stackPointer);
	if (process == (activeProcess())) {
		/* stackTopPut: */
		longAtput(stackPointer,nilObj);

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
		primFailCode = PrimErrBadReceiver;
		return;
	}
	ok = removeProcessfromList(process, myList);
	if (!ok) {
		/* primitiveFailFor: */
		primFailCode = PrimErrOperationFailed;
		return;
	}
	if (((longAt((void *)(myList))) & (classIndexMask())) != classLinkedListClassTag) {
		backupContexttoBlockingSendTo(myContext, myList);

		/* stackTopPut: */
		longAtput(stackPointer,nilObj);
	}
	else {
		/* stackTopPut: */
		longAtput(stackPointer,myList);
	}
}
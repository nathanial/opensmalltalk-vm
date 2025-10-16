/* Extracted from interp.c:51611 (function createActualMessageTo). */

static NoDbgRegParms void
createActualMessageTo(sqInt lookupClass)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt argumentArray;
    sqInt i;
    usqInt message;
    usqInt newObj;
    usqInt numBytes;
    char *sp;


	/* This is a useful break-point */
	assert((isImmediate(GIV(messageSelector)))
	 || (addressCouldBeObj(GIV(messageSelector))));

	/* begin mnuBreakpoint:receiver: */
	mnuBreakpointreceiver(firstFixedFieldOfMaybeImmediate(GIV(messageSelector)), lengthOfMaybeImmediate(GIV(messageSelector)), null);
	assert((GIV(argumentCount) >= 0)
	 && ((knownClassAtIndex(ClassArrayCompactIndex)) != GIV(nilObj)));
	assert((arrayFormat()) == (instSpecOfClass(knownClassAtIndex(ClassArrayCompactIndex))));

	/* begin allocateSmallNewSpaceSlots:format:classIndex: */
	assert(GIV(argumentCount) < (numSlotsMask()));
	newObj = GIV(freeStart);
	numBytes = BaseHeaderSize + ((GIV(argumentCount) < 1
		? 8 /* allocationUnit */
		: GIV(argumentCount) * BytesPerOop));
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
		if (!GIV(needGCFlag)) {
			/* begin scheduleScavenge */
			GIV(needGCFlag) = 1;
			forceInterruptCheck();
		}
		if ((GIV(freeStart) + numBytes) > (((GIV(eden)).limit))) {
			error("no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
			argumentArray = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) GIV(argumentCount))) << (numSlotsFullShift()))) + ((((usqInt)((arrayFormat())) << (formatShift()))))) + ClassArrayCompactIndex);
	GIV(freeStart) += numBytes;
	argumentArray = newObj;
	/* end eeInstantiateSmallClassIndex:format:numSlots: */
l1:

	/* begin eeInstantiateSmallClassIndex:format:numSlots: */
	assert(((MessageLookupClassIndex + 1) >= 0)
	 && ((knownClassAtIndex(ClassMessageCompactIndex)) != GIV(nilObj)));
	assert((nonIndexablePointerFormat()) == (instSpecOfClass(knownClassAtIndex(ClassMessageCompactIndex))));

	/* begin allocateSmallNewSpaceSlots:format:classIndex: */
	assert((MessageLookupClassIndex + 1) < (numSlotsMask()));
	newObj = GIV(freeStart);
	numBytes = BaseHeaderSize + (((MessageLookupClassIndex + 1) < 1
		? 8 /* allocationUnit */
		: (MessageLookupClassIndex + 1) * BytesPerOop));
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
		if (!GIV(needGCFlag)) {
			/* begin scheduleScavenge */
			GIV(needGCFlag) = 1;
			forceInterruptCheck();
		}
		if ((GIV(freeStart) + numBytes) > (((GIV(eden)).limit))) {
			error("no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
			message = 0;
			goto l2;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) (MessageLookupClassIndex + 1))) << (numSlotsFullShift()))) + ((((usqInt)((nonIndexablePointerFormat())) << (formatShift()))))) + ClassMessageCompactIndex);
	GIV(freeStart) += numBytes;
	message = newObj;
	/* end eeInstantiateSmallClassIndex:format:numSlots: */
l2:

	/* Since the array is new can use unchecked stores. */
	for (i = ((GIV(argumentCount) - 1) * BytesPerOop); i >= 0; i += (-BytesPerOop)) {
		longAtput((void *)((argumentArray + BaseHeaderSize) + i),popStack());
	}

	/* Since message is new can use unchecked stores. */

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(message))
	 && (!(isForwarded(message))));
	assert(validStorePointerUncheckedArgs(MessageSelectorIndex, message, GIV(messageSelector)));
	longAtput((void *)((message + BaseHeaderSize) + ((((usqInt)(MessageSelectorIndex) << (shiftForWord()))))),GIV(messageSelector));

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(message))
	 && (!(isForwarded(message))));
	assert(validStorePointerUncheckedArgs(MessageArgumentsIndex, message, argumentArray));
	longAtput((void *)((message + BaseHeaderSize) + ((((usqInt)(MessageArgumentsIndex) << (shiftForWord()))))),argumentArray);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(message))
	 && (!(isForwarded(message))));
	assert(validStorePointerUncheckedArgs(MessageLookupClassIndex, message, lookupClass));
	longAtput((void *)((message + BaseHeaderSize) + ((((usqInt)(MessageLookupClassIndex) << (shiftForWord()))))),lookupClass);

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),message);
	GIV(stackPointer) = sp;
	GIV(argumentCount) = 1;
}
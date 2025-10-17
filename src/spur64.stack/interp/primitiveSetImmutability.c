/* Extracted from interp.c:22052 (function primitiveSetImmutability). */

static void
primitiveSetImmutability(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt format;
    sqInt processLists;
    sqInt rcvr;
    sqInt scheduler;
    char *sp;
    sqInt wasImmutable;

	rcvr = longAt(stackPointer + (1 * BytesPerWord));
	if (((rcvr & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadReceiver;
		return;
	}
	if ((argumentCount > 1)
	 && ((!((longAt((void *)(rcvr))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}

	/* fail if forwarded; the retry machinery will fix things up. */
	wasImmutable = 
#    if IMMUTABILITY
			((((usqInt)((byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0
#    else
			0
#    endif
			;
	if ((longAt(stackPointer)) == trueObj) {
		/* begin canBeImmutable: */
		assert(isNonImmediate(rcvr));

		/* For now we fail the primitive for contexts to we ensure there are no immutable contexts.
		   Later we can consider having immutable contexts and send cannotReturn callback
		   when returning to an immutable context. That would mean that setting a context
		   to immutable would require a divorce and returns to immutable context are
		   necessarily across stack pages */
		if (((longAt((void *)(rcvr))) & (classIndexMask())) == ClassMethodContextCompactIndex) {
			goto l1;
		}

		/* Weak structures can't be immutable */

		/* begin isEphemeron: */
		assert(isNonImmediate(rcvr));
		format = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
		if (format == (ephemeronFormat())) {
			goto l1;
		}

		/* begin isWeakNonImm: */
		format = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
		if (format == (weakArrayFormat())) {
			goto l1;
		}

		/* No clue what is going on for semaphores so they can't be immutable */
		if (((longAt((void *)(rcvr))) & (classIndexMask())) == (rawHashBitsOf(longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassSemaphore) << (shiftForWord()))))))))) {
			goto l1;
		}

		/* Simple version of process management: we forbid Process and LinkedList instances to be immutable
		   as well as the Processor and the array of activeProcess */

		/* begin fetchPointer:ofObject: */
		scheduler = longAt((void *)(((longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
		processLists = longAt((void *)((scheduler + BaseHeaderSize) + ((((usqInt)(ProcessListsIndex) << (shiftForWord()))))));
		if (rcvr == scheduler) {
			goto l1;
		}
		if (rcvr == processLists) {
			goto l1;
		}

		/* Is it a linkedList ? */
		if ((classIndexOf(longAt((void *)((processLists + BaseHeaderSize) + (1U << (shiftForWord())))))) == ((longAt((void *)(rcvr))) & (classIndexMask()))) {
			goto l1;
		}

		/* is it a Process ? */
		if ((classIndexOf(longAt((void *)((scheduler + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord())))))))) == ((longAt((void *)(rcvr))) & (classIndexMask()))) {
			goto l1;
		}

		/* The rest of the code is relative to process management: the Processor (the active
		   process scheduler) can't be immutable, as well as all the objects relative to Process management
		   scheduler := self fetchPointer: ValueIndex ofObject: (self splObj: SchedulerAssociation).
		   processLists := objectMemory fetchPointer: ProcessListsIndex ofObject: scheduler.
		   ((objectMemory formatOf: oop) = objectMemory nonIndexablePointerFormat)
		   ifFalse:
		   [ (objectMemory isArrayNonImm: oop) ifFalse: [^ true].
		   ^ (oop = processLists) not ].
		   (objectMemory numSlotsOf: oop) >= 2 ifFalse: [^ true].
		   
		   is the oop the scheduler itself ?
		   oop = scheduler ifTrue: [^ false].
		   1 to: (objectMemory numSlotsOf: processLists) do: [ :i |
		   
		   is the oop one of the linked lists ?
		   (list := processLists at: i) = oop ifTrue: [^ false].
		   
		   is the oop one of the runnable process ?
		   first := objectMemory fetchPointer: FirstLinkIndex ofObject: list.
		   first = objectMemory nilObject ifFalse:
		   [ last := objectMemory fetchPointer: LastLinkIndex ofObject: list.
		   link := first.
		   [ link = last ] whileFalse:
		   [ link = oop ifTrue: [^ false].
		   link := objectMemory fetchPointer: NextLinkIndex ofObject: link. ] ] ]. */
		goto l2;
l1:

		/* primitiveFailFor: */
		primFailCode = PrimErrInappropriate;
		return;
l2:

		/* begin setIsImmutableOf:to: */
		longAtput((void *)(rcvr),(longAt((void *)(rcvr))) | (1U << (immutableBitShift())));
	}
	else {
		if ((longAt(stackPointer)) == falseObj) {
			/* begin setIsImmutableOf:to: */
			longAtput((void *)(rcvr),(longAt((void *)(rcvr))) & (~(usqIntptr_t)(1U << (immutableBitShift()))));
		}
		else {
			/* primitiveFailFor: */
			primFailCode = PrimErrBadArgument;
			return;
		}
	}

	/* begin methodReturnBool: */
	assert(!((failed())));
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
		(wasImmutable
			? trueObj
			: falseObj));
	stackPointer = sp;
}
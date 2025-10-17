/* Extracted from interp.c:67680 (function primitiveDoPrimitiveWithArgs). */

/*	Implement either ProtoObject>>tryPrimitive: primIndex withArgs: argArray
	or Context>>receiver: anObject tryPrimitive: primIndex withArgs: argArray.
	If this primitive fails, arrange that its error code is a negative
	integer, to
	distinguish between this failing and the primitive it invokes failing. */

	/* StackInterpreterPrimitives>>#primitiveDoPrimitiveWithArgs */

static void
primitiveDoPrimitiveWithArgs(void)
{
    sqInt argumentArray;
    usqInt arraySize;
    sqInt index;
    usqInt numSlots;
    sqInt primIdx;
    sqInt rcvr;
    sqInt referent;
    sqInt savedNumArgs;
    char *sp;

	metaAccessorDepth = -2;

	/* See checkForAndFollowForwardedPrimitiveState */
	if (!(((argumentCount >= 2) && (argumentCount <= 3)))) {
		/* primitiveFailFor: */
		primFailCode = -PrimErrUnsupported;
		return;
	}
	argumentArray = longAt(stackPointer);
	primIdx = longAt(stackPointer + (1 * BytesPerWord));
	if (!((/* isArray: */
			((!(argumentArray & (tagMask()))))
		 && (((byteAt((void *)(argumentArray + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))
		 && ((((primIdx) & 7) == 1)))) {
		/* primitiveFailFor: */
		primFailCode = -PrimErrBadArgument;
		return;
	}

	/* begin numSlotsOf: */
	assert((classIndexOf(argumentArray)) > (isForwardedObjectClassIndexPun()));
	arraySize = (((numSlots = byteAt((void *)(argumentArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(argumentArray - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	if (!(arraySize <= (LargeContextSlots - CtxtTempFrameStart))) {
		/* primitiveFailFor: */
		primFailCode = -PrimErrLimitExceeded;
		return;
	}
	primIdx = (primIdx >> 3);

	/* begin functionPointerFor:inClass: */
	primitiveFunctionPointer = ((void (*)(void)) (((((usqInt)primIdx)) > MaxPrimitiveIndex
		? 0
		: primitiveTable[primIdx])));
	if (!primitiveFunctionPointer) {
		primitiveFunctionPointer = primitiveDoPrimitiveWithArgs;

		/* primitiveFailFor: */
		primFailCode = -PrimErrBadIndex;
		return;
	}

	/* Pop primIndex and argArray, then push args in place... */
	if (((savedNumArgs = argumentCount)) == 3) {
		/* actual receiver */
		tempOop2 = longAt(stackPointer + (3 * BytesPerWord));

		/* receiver for primitive */
		rcvr = longAt(stackPointer + (2 * BytesPerWord));
		if (/* isOopForwarded: */
			((!(rcvr & (tagMask()))))
		 && ((!((longAt((void *)(rcvr))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			/* begin followForwarded: */
			assert(isUnambiguouslyForwarder(rcvr));
			referent = longAt((void *)((rcvr + BaseHeaderSize) + (0U << (shiftForWord()))));
			while (/* isOopForwarded: */
				((!(referent & (tagMask()))))
			 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
			}
			rcvr = referent;
		}

		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (3 * BytesPerWord)),rcvr);
		stackPointer = sp;
	}
	else {
		/* begin pop: */
		stackPointer += 2 * BytesPerWord;
	}

	/* ...and receiver if the three arg form */
	argumentCount = arraySize;
	index = 1;
	while (index <= arraySize) {
		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),longAt((void *)((argumentArray + BaseHeaderSize) + ((((usqInt)((index - 1)) << (shiftForWord())))))));
		stackPointer = sp;
		index += 1;
	}
	if ((((usqIntptr_t) primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex) {
		externalQuickPrimitiveResponse();
		tempOop2 = 0;
		return;
	}

	/* We use tempOop instead of pushRemappableOop:/popRemappableOop here because in
	   the Cogit primitiveEnterCriticalSection, primitiveSignal, primitiveResume et al longjmp back
	   to either the interpreter or machine code, depending on the process activated.  So if we're
	   executing one of these primitives, control won't actually return here and the matching
	   popRemappableOop: wouldn't occur, potentially overflowing the remap buffer.
	   Note that while recursion could occur (nil tryPrimitive: 118 withArgs: #(118 #(110 #())))
	   it counts as shooting oneself in the foot. */

	/* prim might alloc/gc */
	/* Run the primitive (sets primFailCode) */
	tempOop = argumentArray;
	metaAccessorDepth = primitiveAccessorDepthTable[primIdx];

	/* See checkForAndFollowForwardedPrimitiveState */
	if (!(slowPrimitiveResponse())) {
		if (!tempOop) {
			return;
		}

		/* the primitive failed in a recursive invocation.  can't fix things with no value... */

		/* begin pop: */
		stackPointer += arraySize * BytesPerWord;
		if (savedNumArgs == 3) {
			rcvr = longAt(stackPointer);

			/* stackTopPut: */
			longAtput(stackPointer,tempOop2);

			/* begin push: */
			longAtput((sp = stackPointer - BytesPerWord),rcvr);
			stackPointer = sp;
		}

		/* begin pushInteger: */
		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),(((usqInt)primIdx << 3) | 1));
		stackPointer = sp;

		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),tempOop);
		stackPointer = sp;
		primitiveFunctionPointer = primitiveDoPrimitiveWithArgs;
		argumentCount = savedNumArgs;
	}

	/* If primitive failed, then restore state for failure code */
	tempOop = (tempOop2 = 0);
}
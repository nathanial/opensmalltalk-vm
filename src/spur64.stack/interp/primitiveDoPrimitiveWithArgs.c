/* Extracted from interp.c:67680 (function primitiveDoPrimitiveWithArgs). */

/*	Implement either ProtoObject>>tryPrimitive: primIndex withArgs: argArray
	or Context>>receiver: anObject tryPrimitive: primIndex withArgs: argArray.
	If this primitive fails, arrange that its error code is a negative
	integer, to
	distinguish between this failing and the primitive it invokes failing. */

	/* StackInterpreterPrimitives>>#primitiveDoPrimitiveWithArgs */

static void
primitiveDoPrimitiveWithArgs(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt argumentArray;
    usqInt arraySize;
    sqInt index;
    usqInt numSlots;
    sqInt primIdx;
    sqInt rcvr;
    sqInt referent;
    sqInt savedNumArgs;
    char *sp;

	GIV(metaAccessorDepth) = -2;

	/* See checkForAndFollowForwardedPrimitiveState */
	if (!(((GIV(argumentCount) >= 2) && (GIV(argumentCount) <= 3)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -PrimErrUnsupported;
		return;
	}
	argumentArray = longAt(GIV(stackPointer));
	primIdx = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (!((/* isArray: */
			((!(argumentArray & (tagMask()))))
		 && (((byteAt((void *)(argumentArray + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))
		 && ((((primIdx) & 7) == 1)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -PrimErrBadArgument;
		return;
	}

	/* begin numSlotsOf: */
	assert((classIndexOf(argumentArray)) > (isForwardedObjectClassIndexPun()));
	arraySize = (((numSlots = byteAt((void *)(argumentArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(argumentArray - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	if (!(arraySize <= (LargeContextSlots - CtxtTempFrameStart))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -PrimErrLimitExceeded;
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
		GIV(primFailCode) = -PrimErrBadIndex;
		return;
	}

	/* Pop primIndex and argArray, then push args in place... */
	if (((savedNumArgs = GIV(argumentCount))) == 3) {
		/* actual receiver */
		GIV(tempOop2) = longAt(GIV(stackPointer) + (3 * BytesPerWord));

		/* receiver for primitive */
		rcvr = longAt(GIV(stackPointer) + (2 * BytesPerWord));
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
		longAtput((sp = GIV(stackPointer) + (3 * BytesPerWord)),rcvr);
		GIV(stackPointer) = sp;
	}
	else {
		/* begin pop: */
		GIV(stackPointer) += 2 * BytesPerWord;
	}

	/* ...and receiver if the three arg form */
	GIV(argumentCount) = arraySize;
	index = 1;
	while (index <= arraySize) {
		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),longAt((void *)((argumentArray + BaseHeaderSize) + ((((usqInt)((index - 1)) << (shiftForWord())))))));
		GIV(stackPointer) = sp;
		index += 1;
	}
	if ((((usqIntptr_t) primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex) {
		externalQuickPrimitiveResponse();
		GIV(tempOop2) = 0;
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
	GIV(tempOop) = argumentArray;
	GIV(metaAccessorDepth) = primitiveAccessorDepthTable[primIdx];

	/* See checkForAndFollowForwardedPrimitiveState */
	if (!(slowPrimitiveResponse())) {
		if (!GIV(tempOop)) {
			return;
		}

		/* the primitive failed in a recursive invocation.  can't fix things with no value... */

		/* begin pop: */
		GIV(stackPointer) += arraySize * BytesPerWord;
		if (savedNumArgs == 3) {
			rcvr = longAt(GIV(stackPointer));

			/* stackTopPut: */
			longAtput(GIV(stackPointer),GIV(tempOop2));

			/* begin push: */
			longAtput((sp = GIV(stackPointer) - BytesPerWord),rcvr);
			GIV(stackPointer) = sp;
		}

		/* begin pushInteger: */
		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),(((usqInt)primIdx << 3) | 1));
		GIV(stackPointer) = sp;

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(tempOop));
		GIV(stackPointer) = sp;
		primitiveFunctionPointer = primitiveDoPrimitiveWithArgs;
		GIV(argumentCount) = savedNumArgs;
	}

	/* If primitive failed, then restore state for failure code */
	GIV(tempOop) = (GIV(tempOop2) = 0);
}
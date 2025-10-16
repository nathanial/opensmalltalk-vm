/* Extracted from interp.c:20681 (function primitivePerformWithArgs). */

static void
primitivePerformWithArgs(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt actualReceiver;
    sqInt arg;
    sqInt argumentArray;
    usqInt arraySize;
    sqInt delta;
    sqInt index;
    usqInt numSlots;
    sqInt offset;
    sqInt performArgCount;
    usqInt savedNewMethod;
    sqInt selector;
    char *sp;
    sqInt tagBits;
    sqInt top;

	actualReceiver = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	selector = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	argumentArray = longAt(GIV(stackPointer));

	/* begin primitiveObject:perform:withArguments:lookedUpIn: */
	if (!(/* isArray: */
			((!(argumentArray & (tagMask()))))
		 && (((byteAt((void *)(argumentArray + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))) {
		GIV(performWithArgumentsRecursionGuard) = null;

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		goto l1;
	}

	/* Check if number of arguments is reasonable; MaxNumArgs isn't available
	   so just use LargeContextSize */

	/* begin numSlotsOf: */
	assert((classIndexOf(argumentArray)) > (isForwardedObjectClassIndexPun()));
	arraySize = (((numSlots = byteAt((void *)(argumentArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(argumentArray - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	if (arraySize > (LargeContextSlots - CtxtTempFrameStart)) {
		GIV(performWithArgumentsRecursionGuard) = null;

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadNumArgs;
		goto l1;
	}
	performArgCount = GIV(argumentCount);

	/* Recursion check:
	   | a |
	   a := Array with: #perform:withArguments: with: nil.
	   a at: 2 put: a.
	   a perform: a first withArguments: a.
	   | a |
	   a := Array with: #perform:withArguments: with: (Array with: #perform:withArguments: with: nil).
	   a last at: 2 put: a.
	   a perform: a first withArguments: a */
	if (!GIV(performWithArgumentsRecursionGuard)) {
		GIV(performWithArgumentsRecursionGuard) = longAt(GIV(stackPointer));
	}

	/* Push newMethod to save it in case of failure,
	   then push the actual receiver and the args in the array. */
	savedNewMethod = GIV(newMethod);

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(newMethod));
	GIV(stackPointer) = sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),actualReceiver);
	GIV(stackPointer) = sp;

	/* Copy the arguments to the stack, in case of MNU, and lookup */
	for (index = 1; index <= arraySize; index += 1) {
		arg = longAt((void *)((argumentArray + BaseHeaderSize) + ((((usqInt)((index - 1)) << (shiftForWord()))))));
		if (arg == GIV(performWithArgumentsRecursionGuard)) {
			GIV(performWithArgumentsRecursionGuard) = null;
		}

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),arg);
		GIV(stackPointer) = sp;
	}
	if (!GIV(performWithArgumentsRecursionGuard)) {
		/* begin pop: */
		GIV(stackPointer) += (2 + arraySize) * BytesPerWord;

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrInappropriate;
		goto l1;
	}
	GIV(argumentCount) = arraySize;
	GIV(messageSelector) = selector;

	/* begin sendBreakpoint:receiver: */
	sendBreakpointclassTag(firstFixedFieldOfMaybeImmediate(GIV(messageSelector)), lengthOfMaybeImmediate(GIV(messageSelector)), /* fetchClassTagOf: */
		((tagBits = actualReceiver & (tagMask()))
			? tagBits
			: (longAt((void *)(actualReceiver))) & (classIndexMask())));
	if (
#  if SEND_PRINTING
		GIV(printSends)
#  else
		0
#  endif
		) {
		printActivationNameForSelectorstartClass(GIV(messageSelector), /* fetchClassOf: */
			((tagBits = actualReceiver & (tagMask()))
				? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
				: fetchClassOfNonImm(actualReceiver)));
		cr();
	}
	findNewMethodInClassTag(/* fetchClassTagOf: */
		((tagBits = actualReceiver & (tagMask()))
			? tagBits
			: (longAt((void *)(actualReceiver))) & (classIndexMask())));

	/* Only test CompiledMethods for argument count - any other objects playacting as CMs will have to take their chances */
	if ((/* isOopCompiledMethod: */
		((!(GIV(newMethod) & (tagMask()))))
	 && (((byteAt((void *)(GIV(newMethod) + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))
	 && ((argumentCountOf(GIV(newMethod))) != GIV(argumentCount))) {
		assert((stackTop()) == ((arraySize == 0
				? actualReceiver
				: fetchPointerofObject(arraySize - 1, argumentArray))));
		assert(GIV(argumentCount) == arraySize);

		/* begin pop: */
		GIV(stackPointer) += (arraySize + 1) * BytesPerWord;

		/* begin popStack */
		top = longAt(GIV(stackPointer));
		GIV(stackPointer) += BytesPerWord;
		GIV(newMethod) = top;

		/* Must reset primitiveFunctionPointer for checkForAndFollowForwardedPrimitiveState */
		GIV(argumentCount) = performArgCount;
		primitiveFunctionPointer = primitivePerformWithArgs;
		GIV(performWithArgumentsRecursionGuard) = null;

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadNumArgs;
		goto l1;
	}

	/* Restore the state by popping the array entries, the actual receiver and the saved
	   newMethod, leaving the selector and array, and fail.  N.B.  If an MNU has happened
	   then argumentCount will match newMethod, so this code will not be reached.
	   These asserts check that an MNU has not occurred if the argumentCount doesn't match the newMethod.
	   Cannot fail this primitive from here-on.  Slide the actual receiver and arguments down
	   to replace the perform arguments and saved newMethod and then execute the new
	   method. Use argumentCount not arraySize because an MNU may have changed it. */

	/* +2 = receiver + saved newMethod */
	delta = BytesPerWord * (performArgCount + 2);
	for (offset = (GIV(argumentCount) * BytesPerWord); offset >= 0; offset += (-BytesPerWord)) {
		longAtput((GIV(stackPointer) + offset) + delta,longAt(GIV(stackPointer) + offset));
	}

	/* Part of the recursion guard above. To ensure we fail in the original method, restore
	   newMethod, and don't do the activateNewMethod implicit in executeNewMethod. */
	if ((primitiveFunctionPointer == primitivePerformWithArgs)
	 || (primitiveFunctionPointer == primitivePerformInSuperclass)) {
		GIV(newMethod) = savedNewMethod;
		GIV(argumentCount) = performArgCount;

		/* begin pop: */
		GIV(stackPointer) += (performArgCount + 2) * BytesPerWord;

		/* prevent inlining... */
		slowPrimitiveResponse();
		GIV(performWithArgumentsRecursionGuard) = null;
		goto l1;
	}

	/* This should of course be a tail call, which could be done via setjmp/longjmp.
	   But this is vanity code. After all how often is a recursive invocation of
	   primitivePerformWithArgs et al made? */
	GIV(performWithArgumentsRecursionGuard) = null;

	/* begin pop: */
	GIV(stackPointer) += (performArgCount + 2) * BytesPerWord;

	/* prevent inlining... */
	executeNewMethod();

	/* begin initPrimCall */
	GIV(primFailCode) = 0;
	/* end primitiveObject:perform:withArguments:lookedUpIn: */
l1:;
}
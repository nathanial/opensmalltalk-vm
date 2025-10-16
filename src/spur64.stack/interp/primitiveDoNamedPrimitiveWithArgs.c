/* Extracted from interp.c:67390 (function primitiveDoNamedPrimitiveWithArgs). */

static void
primitiveDoNamedPrimitiveWithArgs(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    void (*addr)(void);
    sqInt argumentArray;
    usqInt arraySize;
    sqInt ccIndex;
    usqInt firstBytecode;
    sqInt fmt;
    sqInt functionLength;
    sqInt functionName;
    sqInt index;
    sqInt isArray;
    sqInt methodArg;
    sqInt methodHeader;
    sqInt moduleLength;
    sqInt moduleName;
    usqInt newObj;
    usqInt numBytes;
    usqInt numSlots;
    sqInt primRcvr;
    char *sp;
    sqInt spec;
    sqInt successBoolean;
    sqInt top;
    sqInt valuePointer;

	GIV(metaAccessorDepth) = -2;

	/* See checkForAndFollowForwardedPrimitiveState */
	argumentArray = longAt(GIV(stackPointer));
	methodArg = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	if (!((/* isArray: */
			((!(argumentArray & (tagMask()))))
		 && (((byteAt((void *)(argumentArray + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))
		 && (/* isOopCompiledMethod: */
			((!(methodArg & (tagMask()))))
		 && (((byteAt((void *)(methodArg + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat()))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -2;
		return;
	}

	/* begin numSlotsOf: */
	assert((classIndexOf(argumentArray)) > (isForwardedObjectClassIndexPun()));
	arraySize = (((numSlots = byteAt((void *)(argumentArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(argumentArray - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	if (!(arraySize <= (LargeContextSlots - CtxtTempFrameStart))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -2;
		return;
	}

	/* begin methodHeaderOf: */
	assert(isCompiledMethod(methodArg));
	methodHeader = longAt((void *)((methodArg + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	if (!(((/* begin literalCountOfMethodHeader: */
			assert((((methodHeader) & 7) == 1)),
		/* literalCountOfAlternateHeader: */
			((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) > 2)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -3;
		return;
	}
	spec = longAt((void *)((methodArg + BaseHeaderSize) + (1U << (shiftForWord()))));

	/* first literal */

	/* begin isInstanceOfClassArray: */
	/* begin is:instanceOf:compactClassIndex: */
	if (((spec & (tagMask())) != 0)) {
		isArray = 0;
		goto l2;
	}

	/* begin isClassOfNonImm:equalTo:compactClassIndex: */
	assert(!(isImmediate(spec)));
	ccIndex = (longAt((void *)(spec))) & (classIndexMask());
	isArray = ClassArrayCompactIndex == ccIndex;
	/* end isInstanceOfClassArray: */
l2:
	if (!(isArray
		 && ((((/* begin numSlotsOf: */
			assert((classIndexOf(spec)) > (isForwardedObjectClassIndexPun())),
		(((numSlots = byteAt((void *)(spec + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(spec - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots))) == 4)
		 && ((/* primitiveIndexOfMethod:header: */
			(((methodHeader & AlternateHeaderHasPrimFlag) != 0)
				? ((firstBytecode = (methodArg + ((LiteralStart + (((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) * BytesPerOop)) + BaseHeaderSize),
				(byteAt((void *)(firstBytecode + 1))) + ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8))))
				: 0)) == PrimNumberExternalCall)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -3;
		return;
	}
	if (!(((((usqInt)(methodHeader)) >> MethodHeaderArgCountShift) & 15) == arraySize)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -2;
		return;
	}
	moduleName = longAt((void *)((spec + BaseHeaderSize) + (0U << (shiftForWord()))));
	if (moduleName == GIV(nilObj)) {
		moduleLength = 0;
	}
	else {
		successBoolean = /* isBytes: */
				((!(moduleName & (tagMask()))))
			 && (((byteAt((void *)(moduleName + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat()));

		/* begin success: */
		if (!successBoolean) {
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
		}

		/* begin numBytesOfBytes: */
		fmt = (byteAt((void *)(moduleName + (formatFieldByteOffset())))) & (formatMask());
		assert(fmt >= (firstByteFormat()));
		moduleLength = ((((/* begin numSlotsOf: */
	assert((classIndexOf(moduleName)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(moduleName + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(moduleName - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);
	}
	functionName = longAt((void *)((spec + BaseHeaderSize) + (1U << (shiftForWord()))));
	successBoolean = /* isBytes: */
			((!(functionName & (tagMask()))))
		 && (((byteAt((void *)(functionName + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat()));

	/* begin success: */
	if (!successBoolean) {
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}

	/* begin numBytesOfBytes: */
	fmt = (byteAt((void *)(functionName + (formatFieldByteOffset())))) & (formatMask());
	assert(fmt >= (firstByteFormat()));
	functionLength = ((((/* begin numSlotsOf: */
	assert((classIndexOf(functionName)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(functionName + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(functionName - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) - (fmt & 7);
	if (GIV(primFailCode)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -3;
		return;
	}
	addr = ioLoadExternalFunctionOfLengthFromModuleOfLengthMetadataInto(functionName + BaseHeaderSize, functionLength, moduleName + BaseHeaderSize, moduleLength, (&GIV(metaAccessorDepth)));

	/* N.B. the accessor depth is the second byte of the primitive's metadata;
	   the first byte is various flags (currently l.s.b. = use fast C linkage). */
	GIV(metaAccessorDepth) = (addr
				? (((usqInt)(GIV(metaAccessorDepth))) >> 8)
				: -2);
	if (!addr) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -1;
		return;
	}

	/* begin eeInstantiateClassIndex:format:numSlots: */
	assert((knownClassAtIndex(ClassArrayCompactIndex)) != GIV(nilObj));
	assert((arrayFormat()) == (instSpecOfClass(knownClassAtIndex(ClassArrayCompactIndex))));
	newObj = GIV(freeStart);
	numBytes = BaseHeaderSize + (4 * BytesPerOop);
	if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
		if (!GIV(needGCFlag)) {
			/* begin scheduleScavenge */
			GIV(needGCFlag) = 1;
			forceInterruptCheck();
		}
		if ((GIV(freeStart) + numBytes) > (((GIV(eden)).limit))) {
			error("no room in eden for allocateNewSpaceSlots:format:classIndex:");
			GIV(tempOop) = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) 4)) << (numSlotsFullShift()))) + ((((usqInt)((arrayFormat())) << (formatShift()))))) + ClassArrayCompactIndex);

	/* for header parsing we put a saturated slot count in the prepended overflow size word */
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	GIV(freeStart) += numBytes;
	GIV(tempOop) = newObj;
	/* end eeInstantiateClassIndex:format:numSlots: */
l1:

	/* begin popStack */
	valuePointer = (argumentArray = longAt(GIV(stackPointer)));
	GIV(stackPointer) += BytesPerWord;
	valuePointer = (argumentArray);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(GIV(tempOop)))
	 && (!(isForwarded(GIV(tempOop)))));
	assert(validStorePointerUncheckedArgs(0, GIV(tempOop), valuePointer));
	longAtput((void *)((GIV(tempOop) + BaseHeaderSize) + (0U << (shiftForWord()))),valuePointer);

	/* begin popStack */
	valuePointer = (primRcvr = longAt(GIV(stackPointer)));
	GIV(stackPointer) += BytesPerWord;
	valuePointer = (primRcvr);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(GIV(tempOop)))
	 && (!(isForwarded(GIV(tempOop)))));
	assert(validStorePointerUncheckedArgs(1, GIV(tempOop), valuePointer));
	longAtput((void *)((GIV(tempOop) + BaseHeaderSize) + (1U << (shiftForWord()))),valuePointer);

	/* begin popStack */
	top = longAt(GIV(stackPointer));
	GIV(stackPointer) += BytesPerWord;
	valuePointer = top;

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(GIV(tempOop)))
	 && (!(isForwarded(GIV(tempOop)))));
	assert(validStorePointerUncheckedArgs(2, GIV(tempOop), valuePointer));
	longAtput((void *)((GIV(tempOop) + BaseHeaderSize) + (2U << (shiftForWord()))),valuePointer);

	/* begin popStack */
	top = longAt(GIV(stackPointer));
	GIV(stackPointer) += BytesPerWord;
	valuePointer = top;

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(GIV(tempOop)))
	 && (!(isForwarded(GIV(tempOop)))));
	assert(validStorePointerUncheckedArgs(3, GIV(tempOop), valuePointer));
	longAtput((void *)((GIV(tempOop) + BaseHeaderSize) + (3U << (shiftForWord()))),valuePointer);

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),primRcvr);
	GIV(stackPointer) = sp;
	GIV(argumentCount) = arraySize;
	for (index = 1; index <= arraySize; index += 1) {
		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),longAt((void *)((argumentArray + BaseHeaderSize) + ((((usqInt)((index - 1)) << (shiftForWord())))))));
		GIV(stackPointer) = sp;
	}

	/* begin callExternalPrimitive: */
	primitiveFunctionPointer = addr;
	dispatchFunctionPointer(addr);
	if (GIV(primFailCode)) {
		/* begin pop: */
		GIV(stackPointer) += (arraySize + 1) * BytesPerWord;

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),longAt((void *)((GIV(tempOop) + BaseHeaderSize) + (3U << (shiftForWord())))));
		GIV(stackPointer) = sp;

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),longAt((void *)((GIV(tempOop) + BaseHeaderSize) + (2U << (shiftForWord())))));
		GIV(stackPointer) = sp;

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),longAt((void *)((GIV(tempOop) + BaseHeaderSize) + (1U << (shiftForWord())))));
		GIV(stackPointer) = sp;

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),longAt((void *)((GIV(tempOop) + BaseHeaderSize) + (0U << (shiftForWord())))));
		GIV(stackPointer) = sp;
		GIV(argumentCount) = 3;

		/* Must reset primitiveFunctionPointer for checkForAndFollowForwardedPrimitiveState */
		primitiveFunctionPointer = primitiveDoNamedPrimitiveWithArgs;

		/* Hack.  A nil prim error code (primErrorCode = 1) is interpreted by the image
		   as meaning this primitive is not implemented.  So to pass back nil as an error
		   code we use -1 to indicate generic failure. */
		if (GIV(primFailCode) == 1) {
			GIV(primFailCode) = -1;
		}
	}
}
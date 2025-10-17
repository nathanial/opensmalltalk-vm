/* Extracted from interp.c:33830 (function copyObjtoAddrstopAtsavedFirstFieldsindex). */

/*	This is part of storeImageSegmentInto:outPointers:roots:.
	Copy objOop into the segment beginning at segAddr, and forward it to the
	copy, saving its first field in savedFirstField, and setting its marked
	bit to indicate it has
	been copied. If it is a class in the class table, set the copy's hash to 0
	for reassignment
	on load, and mark it as a class by setting its isRemembered bit.
	Answer the next segmentAddr if successful. Answer an appropriate error
	code if not */
/*	Copy the object... */

	/* SpurMemoryManager>>#copyObj:toAddr:stopAt:savedFirstFields:index: */

static NoDbgRegParms sqInt
copyObjtoAddrstopAtsavedFirstFieldsindex(sqInt objOop, sqInt segAddr, sqInt endSeg, sqInt savedFirstFields, sqInt i)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt bodySize;
    usqInt contextSize;
    sqInt contextSizeSqInt;
    sqInt copy;
    sqInt fmt;
    sqInt hash;
    sqInt header;
    sqInt iSqInt;
    usqInt numLiterals;
    usqInt numMediatedSlots;
    usqInt numSlots;
    sqInt oop;
    sqInt sp;
    sqInt toDoLimit;
    sqInt valuePointer;

	assert(!((isCopiedIntoSegment(objOop))));
	bodySize = bytesInBody(objOop);
	if (oopisGreaterThanOrEqualTo(segAddr + bodySize, endSeg)) {
		return PrimErrWritePastObject;
	}
	memcpy(((void *)segAddr), ((void *)(/* startOfObject: */
	((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) == (numSlotsMask())
		? objOop - BaseHeaderSize
		: objOop))), bodySize);

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(segAddr + (numSlotsFieldByteOffset())));
	copy = (numSlots == (numSlotsMask())
				? segAddr + BaseHeaderSize
				: segAddr);

	/* Clear remembered, mark bits of all headers copied into the segment (except classes) */
	setIsRememberedOfto(copy, 0);

	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(copy)));
	byteAtput((void *)(copy + (markBitsByteOffset())),(byteAt((void *)(copy + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));

	/* Make any objects with hidden dynamic state (contexts, methods) look like normal objects. */

	/* begin ifAProxy:updateCopy: */
	if (((longAt((void *)(objOop))) & (classIndexMask())) == ClassMethodContextCompactIndex) {
		if (/* isStillMarriedContext: */
			(((((longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(objOop)))) {
			/* begin numSlotsOfMarriedContext: */
			contextSize = stackPointerIndexForFrame(frameOfMarriedContext(objOop));
			assert((CtxtTempFrameStart + contextSize) <= (numSlotsOf(objOop)));
			numMediatedSlots = CtxtTempFrameStart + contextSize;
			for (iSqInt = 0; iSqInt < numMediatedSlots; iSqInt += 1) {
				oop = fetchPointerofMarriedContext(iSqInt, objOop);

				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(copy))
				 && (!(isForwarded(copy))));
				assert(validStorePointerUncheckedArgs(iSqInt, copy, oop));
				longAtput((void *)((copy + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))),oop);
			}
		}
		else {
			/* begin numPointerSlotsOf: */
			fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
			if (fmt <= 5 /* lastPointerFormat */) {
				if ((fmt == (indexablePointersFormat()))
				 && (((longAt((void *)(objOop))) & (classIndexMask())) == ClassMethodContextCompactIndex)) {
					/* begin fetchStackPointerOf: */
					sp = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
					if (!((((sp) & 7) == 1))) {
						contextSizeSqInt = 0;
						goto l1;
					}
					assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(objOop)));
					contextSizeSqInt = (sp >> 3);
					/* end fetchStackPointerOf: */
l1:
					numMediatedSlots = CtxtTempFrameStart + contextSizeSqInt;
					goto l2;
				}

				/* contexts end at the stack pointer */

				/* begin numSlotsOf: */
				assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun()));
				numMediatedSlots = (((numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
							? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
							: numSlots);
				goto l2;
			}
			if (fmt == (forwardedFormat())) {
				numMediatedSlots = 1;
				goto l2;
			}
			if (fmt < (firstCompiledMethodFormat())) {
				numMediatedSlots = 0;
				goto l2;
			}

			/* begin methodHeaderOf: */
			assert(isCompiledMethod(objOop));
			header = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

			/* begin literalCountOfMethodHeader: */
			assert((((header) & 7) == 1));
			numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
			numMediatedSlots = numLiterals + LiteralStart;
			/* end numPointerSlotsOf: */
l2:;
		}

		/* And make sure to nil the slots beyond the top of stack... */
		toDoLimit = ((/* begin numSlotsOf: */
	assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
		for (iSqInt = numMediatedSlots; iSqInt <= toDoLimit; iSqInt += 1) {
			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(copy))
			 && (!(isForwarded(copy))));
			assert(validStorePointerUncheckedArgs(iSqInt, copy, nilObj));
			longAtput((void *)((copy + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))),nilObj);
		}
	}

	/* If the object is a class, zero its identityHash (which is its classIndex) and set its
	   isRemembered bit.  It will be assigned a new hash and entered into the table on load. */
	hash = (long32At((void *)(objOop + 4))) & (identityHashHalfWordMask());
	if ((hash > (lastClassIndexPun()))
	 && ((classOrNilAtIndex(hash)) == objOop)) {
		/* begin setHashBitsOf:to: */
		long32Atput((void *)(copy + 4),((((long32At((void *)(copy + 4))) | (identityHashHalfWordMask())) - (identityHashHalfWordMask()))) + (0));
		setIsRememberedOfto(copy, 1);
	}

	/* Now forward the object to its copy in the segment. */
	valuePointer = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(savedFirstFields))
	 && (!(isForwarded(savedFirstFields))));
	assert(validStorePointerUncheckedArgs(i, savedFirstFields, valuePointer));
	longAtput((void *)((savedFirstFields + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),valuePointer);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(objOop))
	 && (!(isForwarded(objOop))));
	assert(validStorePointerUncheckedArgs(0, objOop, copy));
	longAtput((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))),copy);

	/* begin markAsCopiedIntoSegment: */
	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(objOop)));
	byteAtput((void *)(objOop + (markBitsByteOffset())),(byteAt((void *)(objOop + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));

	/* Answer the new end of segment */
	return segAddr + bodySize;
}
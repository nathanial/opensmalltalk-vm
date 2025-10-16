/* Extracted from interp.c:38249 (function loadImageSegmentFromoutPointers). */

static NoDbgRegParms NeverInline sqInt
loadImageSegmentFromoutPointers(sqInt segmentWordArray, sqInt outPointerArray)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classIndex;
    sqInt classOop;
    sqInt classRef;
    sqInt clone;
    sqInt err;
    sqInt errorCode;
    sqInt errorCodeSqInt;
    sqInt fillIdx;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt followingWordAddressUsqInt;
    sqInt followingWordSqInt;
    sqInt hash;
    sqInt i;
    sqInt loadedObjectsArray;
    usqInt mappedOop;
    sqInt numLoadedObjects;
    usqInt numOutPointers;
    sqInt numSegObjs;
    usqInt numSlots;
    sqInt numSlotsSqInt;
    sqInt objOop;
    sqInt oldClone;
    sqInt oop;
    usqInt oopUsqInt;
    sqInt scanClassTable;
    usqInt segmentLimit;
    sqInt segmentStart;
    sqInt segVersion;
    sqInt toDoLimit;

	/* begin numSlotsOf: */
	assert((classIndexOf(segmentWordArray)) > (isForwardedObjectClassIndexPun()));
	segmentLimit = (((numSlots = byteAt((void *)(segmentWordArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(segmentWordArray - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	if ((/* objectBytesForSlots: */
		(segmentLimit
			? ((segmentLimit << (shiftForWord()))) + ((segmentLimit >= (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize))
			: 8 /* allocationUnit */ + BaseHeaderSize)) < (8 /* allocationUnit */ + BaseHeaderSize)) {
		return PrimErrBadArgument;
	}

	/* version info
	   one object header
	   Verify format.  If the format is wrong, word-swap (since ImageSegment data are 32-bit longs).
	   If it is still wrong, undo the damage and fail. */
	segVersion = longAt((void *)(segmentWordArray + BaseHeaderSize));
	if (!(((((segVersion & 0xFFFFFF) | MultipleBytecodeSetsBitmask) - MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */)) {
		reverseBytesIn32BitWordsIn(segmentWordArray);
		segVersion = longAt((void *)(segmentWordArray + BaseHeaderSize));
		if (!(((((segVersion & 0xFFFFFF) | MultipleBytecodeSetsBitmask) - MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */)) {
			reverseBytesIn32BitWordsIn(segmentWordArray);
			return PrimErrBadArgument;
		}
	}

	/* low 3 bytes */
	segmentStart = (segmentWordArray + BaseHeaderSize) + 8 /* allocationUnit */;
	segmentLimit = ((segmentLimit * BytesPerOop) + segmentWordArray) + BaseHeaderSize;

	/* Notionally reverse the Byte type objects if the data is from opposite endian machine.
	   Test top byte.  $d on the Mac or $s on the PC.  Rest of word is equal.  If Spur is ever
	   ported to big-endian machines then the segment may have to be byte/word swapped,
	   but so far it only runs on little-endian machines, so for now just fail if endianness is wrong. */
	if (((((usqInt)(segVersion)) >> 24) & 0xFF) != ((((usqInt)((imageSegmentVersion()))) >> 24) & 0xFF)) {
		return PrimErrBadArgument;
	}

	/* Reverse the byte-type objects once
	   Avoid having to remember by arranging that there are no young outPointers if segment is in old space. */

	/* begin isOldObject: */
	assert(isNonImmediate(segmentWordArray));
	if (oopisGreaterThanOrEqualTo(segmentWordArray, GIV(oldSpaceStart))) {
		/* begin ensureNoNewObjectsIn: */
		hash = 0;
		scanClassTable = 0;
		toDoLimit = ((/* begin numSlotsOf: */
	assert((classIndexOf(outPointerArray)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(outPointerArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(outPointerArray - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
		for (i = 0; i <= toDoLimit; i += 1) {
			oop = longAt((void *)((outPointerArray + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
			if (/* isYoung: */
				((!(oop & (tagMask()))))
			 && (oopisLessThan(oop, GIV(oldSpaceStart)))) {
				clone = cloneInOldSpaceforPinning(oop, 0);
				if (!clone) {
					errorCode = PrimErrNoMemory;
					goto l6;
				}
				if ((((hash = (long32At((void *)(oop + 4))) & (identityHashHalfWordMask()))) != 0)
				 && ((classOrNilAtIndex(hash)) == oop)) {
					scanClassTable = 1;
				}

				/* begin forward:to: */
				/* begin set:classIndexTo:formatTo: */
				assert((((isForwardedObjectClassIndexPun()) >= 0) && ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
				assert((((forwardedFormat()) >= 0) && ((forwardedFormat()) <= (formatMask()))));
				longAtput((void *)(oop),((longAt((void *)(oop))) & (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) + (classIndexMask())))) + ((isForwardedObjectClassIndexPun()) + ((((usqInt)((forwardedFormat())) << (formatShift()))))));
#        if IMMUTABILITY
				/* begin setIsImmutableOf:to: */
				longAtput((void *)(oop),(longAt((void *)(oop))) & (~(usqIntptr_t)(1U << (immutableBitShift()))));
#        endif // IMMUTABILITY

				assert(isForwarded(oop));
				assert(!(isOopForwarded(clone)));

				/* begin isOldObject: */
				assert(isNonImmediate(oop));
				if (oopisGreaterThanOrEqualTo(oop, GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(clone & (tagMask()))))
					 && (oopisLessThan(clone, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(oop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(oop);
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))),clone);

				/* For safety make sure the forwarder has a slot count that includes its contents. */
				if (!(byteAt((void *)(oop + (numSlotsFieldByteOffset()))))) {
					/* rawNumSlotsOf:put: */
					byteAtput((void *)(oop + (numSlotsFieldByteOffset())),1);
				}

				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(outPointerArray))
				 && (!(isForwarded(outPointerArray))));
				assert(validStorePointerUncheckedArgs(i, outPointerArray, clone));
				longAtput((void *)((outPointerArray + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),clone);
			}
		}
		if (scanClassTable) {
			postBecomeScanClassTable(BecamePointerObjectFlag);
		}
		errorCode = 0;
		/* end ensureNoNewObjectsIn: */
l6:
		if (errorCode) {
			return errorCode;
		}
	}

	/* scan through mapping oops and validating class references. Defer entering any
	   class objects into the class table and/or pinning objects until a second pass. */

	/* begin mapOopsAndValidateClassRefsFrom:to:outPointers: */
	/* begin numSlotsOf: */
	assert((classIndexOf(outPointerArray)) > (isForwardedObjectClassIndexPun()));
	numOutPointers = (((numSlots = byteAt((void *)(outPointerArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(outPointerArray - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	numSegObjs = 0;

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(segmentStart + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? segmentStart + BaseHeaderSize
				: segmentStart);
	while (oopisLessThan(objOop, segmentLimit)) {
		numSegObjs += 1;

		/* No object in the segment should be marked.  If is is something is wrong. */
		if ((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
			errorCode = PrimErrInappropriate;
			goto l8;
		}
		classIndex = (longAt((void *)(objOop))) & (classIndexMask());

		/* validate the class ref, but don't update it until any internal classes have been added to the class table. */
		if (((classIndex & TopHashBit) != 0)) {
			classIndex -= TopHashBit;
			if (classIndex >= numOutPointers) {
				errorCode = PrimErrBadIndex;
				goto l8;
			}
			mappedOop = longAt((void *)((outPointerArray + BaseHeaderSize) + ((((usqInt)(classIndex) << (shiftForWord()))))));
			hash = (long32At((void *)(mappedOop + 4))) & (identityHashHalfWordMask());
			if (!((hash == 0)
				 || ((hash > (lastClassIndexPun()))
				 && ((classOrNilAtIndex(hash)) == mappedOop)))) {
				errorCode = PrimErrInappropriate;
				goto l8;
			}
		}
		else {
			if (((oopUsqInt = ((classIndex - (firstClassIndexPun())) * 8 /* allocationUnit */) + segmentStart)) >= segmentLimit) {
				errorCode = PrimErrBadIndex;
				goto l8;
			}
			if ((long32At((void *)(oopUsqInt + 4))) & (identityHashHalfWordMask())) {
				errorCode = PrimErrInappropriate;
				goto l8;
			}
		}

		/* The class is contained within the segment. */
		toDoLimit = (numPointerSlotsOf(objOop)) - 1;
		for (i = 0; i <= toDoLimit; i += 1) {
			oopUsqInt = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
			if ((!(oopUsqInt & (tagMask())))) {
				if (((oopUsqInt & TopOopBit) != 0)) {
					if (((oopUsqInt = (oopUsqInt - TopOopBit) / BytesPerOop)) >= numOutPointers) {
						errorCode = PrimErrBadIndex;
						goto l8;
					}
					mappedOop = longAt((void *)((outPointerArray + BaseHeaderSize) + ((oopUsqInt << (shiftForWord())))));
				}
				else {
					if (oopUsqInt & 7 /* (allocationUnit - 1) */) {
						errorCode = PrimErrInappropriate;
						goto l8;
					}
					if (((mappedOop = oopUsqInt + segmentStart)) >= segmentLimit) {
						errorCode = PrimErrBadIndex;
						goto l8;
					}
				}

				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(objOop))
				 && (!(isForwarded(objOop))));
				assert(validStorePointerUncheckedArgs(i, objOop, mappedOop));
				longAtput((void *)((objOop + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),mappedOop);
			}
		}

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, segmentLimit)) {
			objOop = segmentLimit;
			goto l7;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l7:;
	}
	errorCode = -numSegObjs;
	/* end mapOopsAndValidateClassRefsFrom:to:outPointers: */
l8:
	if (errorCode > 0) {
		return errorCode;
	}
	numLoadedObjects = -errorCode;
	loadedObjectsArray = noInlineAllocateSlotsformatclassIndex(numLoadedObjects, arrayFormat(), ClassArrayCompactIndex);
	if (!loadedObjectsArray) {
		growOldSpaceByAtLeast((BaseHeaderSize + BaseHeaderSize) + (numLoadedObjects * BytesPerOop));
		loadedObjectsArray = noInlineAllocateSlotsformatclassIndex(numLoadedObjects, arrayFormat(), ClassArrayCompactIndex);
		if (!loadedObjectsArray) {
			return PrimErrNoMemory;
		}
	}

	/* Scan for classes contained in the segment, entering them into the class table.
	   Classes are at the front, after the root array and have the remembered bit set. */

	/* begin enterClassesIntoClassTableFrom:to: */
	objOop = objectAfter(objectStartingAt(segmentStart));
	while ((oopisLessThan(objOop, segmentLimit))
	 && (((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))) != 0)) {
		setIsRememberedOfto(objOop, 0);
		if ((errorCodeSqInt = enterIntoClassTable(objOop))) {
			oop = objOop;
			objOop = objectAfter(objectStartingAt(segmentStart));
			while (oopisLessThan(objOop, oop)) {
				expungeFromClassTable(objOop);

				/* begin objectAfter:limit: */
				followingWordAddress = addressAfter(objOop);
				if (oopisGreaterThanOrEqualTo(followingWordAddress, segmentLimit)) {
					objOop = segmentLimit;
					goto l1;
				}
				followingWord = longAt((void *)(followingWordAddress));
				objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
							? followingWordAddress + BaseHeaderSize
							: followingWordAddress);
				/* end objectAfter:limit: */
l1:;
			}
			errorCode = errorCodeSqInt;
			goto l3;
		}

		/* begin objectAfter:limit: */
		followingWordAddressUsqInt = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddressUsqInt, segmentLimit)) {
			objOop = segmentLimit;
			goto l2;
		}
		followingWordSqInt = longAt((void *)(followingWordAddressUsqInt));
		objOop = ((((usqInt)(followingWordSqInt)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddressUsqInt + BaseHeaderSize
					: followingWordAddressUsqInt);
		/* end objectAfter:limit: */
l2:;
	}
	errorCode = 0;
	/* end enterClassesIntoClassTableFrom:to: */
l3:
	if (errorCode) {
		return errorCode;
	}

	/* Make a final pass, assigning class indices and/or pinning pinned objects and collecting the loaded objects in loadedObjectsArray */

	/* begin assignClassIndicesAndPinFrom:to:outPointers:filling: */
	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(segmentStart + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? segmentStart + BaseHeaderSize
				: segmentStart);
	fillIdx = 0;
	while (oopisLessThan(objOop, segmentLimit)) {
		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(loadedObjectsArray))
		 && (!(isForwarded(loadedObjectsArray))));
		assert(validStorePointerUncheckedArgs(fillIdx, loadedObjectsArray, objOop));
		longAtput((void *)((loadedObjectsArray + BaseHeaderSize) + ((((usqInt)(fillIdx) << (shiftForWord()))))),objOop);
		fillIdx += 1;

		/* In the segment, class indices are offset indexes into the segment data,
		   or into outPointers.  See mapOopsFrom:to:outPointers:outHashes: and
		   newOutPointer:at:in:hashes:. */
		classRef = (longAt((void *)(objOop))) & (classIndexMask());
		classOop = (((classRef & TopHashBit) != 0)
					? longAt((void *)((outPointerArray + BaseHeaderSize) + ((((usqInt)((classRef - TopHashBit)) << (shiftForWord()))))))
					: ((classRef - (firstClassIndexPun())) * 8 /* allocationUnit */) + segmentStart);
		classIndex = (long32At((void *)(classOop + 4))) & (identityHashHalfWordMask());
		if (!classIndex) {
			/* begin ensureBehaviorHash: */
			assert(addressCouldBeClassObj(classOop));

			/* eem 12/28/2021 the above asserft is too weak (and only an assert) */
			classIndex = ((hash = (long32At((void *)(classOop + 4))) & (identityHashHalfWordMask()))
						? hash
						: (objCouldBeClassObj(classOop)
								? ((err = enterIntoClassTable(classOop))
										? -err
										: (long32At((void *)(classOop + 4))) & (identityHashHalfWordMask()))
								: -PrimErrBadReceiver));
			if (classIndex < 0) {
				errorCode = -classIndex;
				goto l5;
			}
		}
		assert((classIndex > (lastClassIndexPun()))
		 && ((classOrNilAtIndex(classIndex)) == classOop));

		/* begin setClassIndexOf:to: */
		assert(((classIndex >= 0) && (classIndex <= (classIndexMask()))));
		longAtput((void *)(objOop),((longAt((void *)(objOop))) & (~(usqIntptr_t)(classIndexMask()))) + classIndex);
		if ((/* isInNewSpace: */
			(oopisLessThan(objOop, GIV(oldSpaceStart)))
		 && (oopisGreaterThanOrEqualTo(objOop, GIV(newSpaceStart))))
		 && (((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)) {
			oldClone = cloneInOldSpaceforPinning(objOop, 1);
			if (oldClone) {
				setIsPinnedOfto(oldClone, 1);

				/* begin forward:to: */
				/* begin set:classIndexTo:formatTo: */
				assert((((isForwardedObjectClassIndexPun()) >= 0) && ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
				assert((((forwardedFormat()) >= 0) && ((forwardedFormat()) <= (formatMask()))));
				longAtput((void *)(objOop),((longAt((void *)(objOop))) & (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) + (classIndexMask())))) + ((isForwardedObjectClassIndexPun()) + ((((usqInt)((forwardedFormat())) << (formatShift()))))));
#        if IMMUTABILITY
				/* begin setIsImmutableOf:to: */
				longAtput((void *)(objOop),(longAt((void *)(objOop))) & (~(usqIntptr_t)(1U << (immutableBitShift()))));
#        endif // IMMUTABILITY

				assert(isForwarded(objOop));
				assert(!(isOopForwarded(oldClone)));

				/* begin isOldObject: */
				assert(isNonImmediate(objOop));
				if (oopisGreaterThanOrEqualTo(objOop, GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(oldClone & (tagMask()))))
					 && (oopisLessThan(oldClone, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(objOop);
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))),oldClone);

				/* For safety make sure the forwarder has a slot count that includes its contents. */
				if (!(byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) {
					/* rawNumSlotsOf:put: */
					byteAtput((void *)(objOop + (numSlotsFieldByteOffset())),1);
				}
			}
		}

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, segmentLimit)) {
			objOop = segmentLimit;
			goto l4;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l4:;
	}
	errorCode = 0;
	/* end assignClassIndicesAndPinFrom:to:outPointers:filling: */
l5:
	if (errorCode) {
		return errorCode;
	}

	/* Evaporate the container, leaving the newly loaded objects in place. */
	if ((byteAt((void *)(segmentWordArray + (numSlotsFieldByteOffset())))) == (numSlotsMask())) {
		if (oopisLessThan(segmentWordArray, GIV(oldSpaceStart))) {
			/* begin rawOverflowSlotsOf:put: */
			longAtput((void *)(segmentWordArray - BaseHeaderSize),((((usqInt)((numSlotsMask())) << 56))));

			/* rawNumSlotsOf:put: */
			byteAtput((void *)(segmentWordArray + (numSlotsFieldByteOffset())),0);
		}
		else {
			numSlotsSqInt = 8 /* allocationUnit */ / BytesPerOop;

			/* begin rawOverflowSlotsOf:put: */
			longAtput((void *)(segmentWordArray - BaseHeaderSize),((((usqInt)((numSlotsMask())) << 56))) + numSlotsSqInt);
		}
	}
	else {
		/* rawNumSlotsOf:put: */
		byteAtput((void *)(segmentWordArray + (numSlotsFieldByteOffset())),8 /* allocationUnit */ / BytesPerOop);
	}

	/* N.B. setting the overflow slots to 1 creates a slimbridge in eden, so we also need to delete the overflow slot count in the segment itself
	   Finally forward the segmentWordArray to the loadedObjectsArray */

	/* begin forward:to: */
	/* begin set:classIndexTo:formatTo: */
	assert((((isForwardedObjectClassIndexPun()) >= 0) && ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
	assert((((forwardedFormat()) >= 0) && ((forwardedFormat()) <= (formatMask()))));
	longAtput((void *)(segmentWordArray),((longAt((void *)(segmentWordArray))) & (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) + (classIndexMask())))) + ((isForwardedObjectClassIndexPun()) + ((((usqInt)((forwardedFormat())) << (formatShift()))))));
#  if IMMUTABILITY
	/* begin setIsImmutableOf:to: */
	longAtput((void *)(segmentWordArray),(longAt((void *)(segmentWordArray))) & (~(usqIntptr_t)(1U << (immutableBitShift()))));
#  endif // IMMUTABILITY

	assert(isForwarded(segmentWordArray));
	assert(!(isOopForwarded(loadedObjectsArray)));

	/* begin isOldObject: */
	assert(isNonImmediate(segmentWordArray));
	if (oopisGreaterThanOrEqualTo(segmentWordArray, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(loadedObjectsArray & (tagMask()))))
		 && (oopisLessThan(loadedObjectsArray, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(segmentWordArray + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(segmentWordArray);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((segmentWordArray + BaseHeaderSize) + (0U << (shiftForWord()))),loadedObjectsArray);

	/* For safety make sure the forwarder has a slot count that includes its contents. */
	if (!(byteAt((void *)(segmentWordArray + (numSlotsFieldByteOffset()))))) {
		/* rawNumSlotsOf:put: */
		byteAtput((void *)(segmentWordArray + (numSlotsFieldByteOffset())),1);
	}
	runLeakCheckerFor(GCModeImageSegment);

	/* Answer the first object in the segment because this is what primitiveLoadImageSegment tests for,
	   even though the real result is the segmentWordArray forwarded to the loadedObjectsArray. */

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(segmentStart + (numSlotsFieldByteOffset())));
	return (numSlots == (numSlotsMask())
			? segmentStart + BaseHeaderSize
			: segmentStart);
}
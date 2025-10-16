/* Extracted from interp.c:39088 (function mapOopsFromtooutPointersoutHashes). */

static NoDbgRegParms sqInt
mapOopsFromtooutPointersoutHashes(sqInt segStart, sqInt segAddr, sqInt outPointerArray, sqInt savedOutHashes)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt hash;
    sqInt heapOop;
    sqInt i;
    usqInt numSlots;
    sqInt numSlotsSqInt;
    usqInt numSlotsUsqInt;
    sqInt objIndex;
    sqInt objOop;
    sqInt oop;
    sqInt outIndex;
    usqInt p;
    sqInt segIndex;
    sqInt toDoLimit;
    usqInt toDoLimitUsqInt;
    sqInt valueWord;
    sqInt valueWordSqInt;

	/* objIndex is for debugging; it mirrors indices in the sender's arrayOfObjects. */
	outIndex = (objIndex = 0);
	/* begin numSlotsOf: */
	assert((classIndexOf(outPointerArray)) > (isForwardedObjectClassIndexPun()));
	if (((numSlots = byteAt((void *)(outPointerArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())) {
		numSlotsSqInt = ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(outPointerArray - BaseHeaderSize)))) << 8)))))) >> 8;
	}
	else {
		numSlotsSqInt = numSlots;
	}

	/* begin fillObj:numSlots:with: */
	assert(oopisLessThan(((outPointerArray + BaseHeaderSize) + (numSlotsSqInt * BytesPerOop)) - 1, addressAfter(outPointerArray)));
	toDoLimitUsqInt = ((usqInt)(((outPointerArray + BaseHeaderSize) + (numSlotsSqInt * BytesPerOop)) - 1));
	for (p = (((usqInt)(outPointerArray + BaseHeaderSize))); p <= toDoLimitUsqInt; p += 8 /* allocationUnit */) {
		longAtput((void *)(p),GIV(nilObj));
	}

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(segStart + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? segStart + BaseHeaderSize
				: segStart);
	while (oopisLessThan(objOop, segAddr)) {
		heapOop = fetchClassOfNonImm(objOop);

		/* Set the classIndex of the instance.  This is a segment offset (segAddr - segStart / allocationUnit) for instances of
		   classes within the segment, and an outPointer index (index in outPointers + TopHashBit) for classes outside the segment. */
		if ((byteAt((void *)(heapOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
			oop = longAt((void *)((heapOop + BaseHeaderSize) + (0U << (shiftForWord()))));
			assert(oopisGreaterThanOrEqualToandLessThan(oop, segStart, segAddr));
			segIndex = ((oop - segStart) / 8 /* allocationUnit */) + (firstClassIndexPun());
			if (((segIndex & TopHashBit) != 0)) {
				return -1;
			}
		}
		else {
			hash = (long32At((void *)(heapOop + 4))) & (identityHashHalfWordMask());
			if (/* is:outPointerClassHashFor:in:limit: */
				(((hash & TopHashBit) != 0))
			 && (((hash - TopHashBit) <= outIndex)
			 && (heapOop == (longAt((void *)((outPointerArray + BaseHeaderSize) + ((((usqInt)((hash - TopHashBit)) << (shiftForWord())))))))))) {
				segIndex = hash;
			}
			else {
				/* begin newOutPointer:at:in:hashes: */
				if (outIndex >= ((/* begin numSlotsOf: */
					assert((classIndexOf(outPointerArray)) > (isForwardedObjectClassIndexPun())),
				(((numSlots = byteAt((void *)(outPointerArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
							? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(outPointerArray - BaseHeaderSize)))) << 8)))))) >> 8
							: numSlots)))) {
					outIndex = 0;
					goto l2;
				}

				/* no room in outPointers; fail */

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(outIndex, outPointerArray, heapOop));
				assert(isNonImmediate(outPointerArray));
				if (oopisGreaterThanOrEqualTo(outPointerArray, GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(heapOop & (tagMask()))))
					 && (oopisLessThan(heapOop, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(outPointerArray + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(outPointerArray);
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((outPointerArray + BaseHeaderSize) + ((((usqInt)(outIndex) << (shiftForWord()))))),heapOop);
				valueWord = (long32At((void *)(heapOop + 4))) & (identityHashHalfWordMask());

				/* begin storeLong32:ofObject:withValue: */
				long32Atput((void *)((savedOutHashes + BaseHeaderSize) + ((((usqInt)(outIndex) << 2)))),valueWord);
				long32Atput((void *)(heapOop + 4),((((long32At((void *)(heapOop + 4))) | (identityHashHalfWordMask())) - (identityHashHalfWordMask()))) + ((outIndex + TopHashBit) & (identityHashHalfWordMask())));
				outIndex += 1;
				/* end newOutPointer:at:in:hashes: */
l2:
				if (!outIndex) {
					return -1;
				}

				/* no room in outPointers; fail */
				segIndex = (long32At((void *)(heapOop + 4))) & (identityHashHalfWordMask());
			}

			/* oop is a new outPointer; allocate its oop */
			assert(((segIndex & TopHashBit) != 0));
		}

		/* oop is a class in the segment; storeImageSegmentInto:outPointers:roots: established offset is within range.
		   oop is an outPointer; locate or allocate its oop */

		/* begin setClassIndexOf:to: */
		assert(((segIndex >= 0) && (segIndex <= (classIndexMask()))));
		longAtput((void *)(objOop),((longAt((void *)(objOop))) & (~(usqIntptr_t)(classIndexMask()))) + segIndex);
		toDoLimit = (numPointerSlotsOf(objOop)) - 1;
		for (i = 0; i <= toDoLimit; i += 1) {
			heapOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
			if ((!(heapOop & (tagMask())))) {
				if ((byteAt((void *)(heapOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
					oop = longAt((void *)((heapOop + BaseHeaderSize) + (0U << (shiftForWord()))));
					assert(oopisGreaterThanOrEqualToandLessThan(oop, segStart, segAddr));
					oop -= segStart;
				}
				else {
					hash = (long32At((void *)(heapOop + 4))) & (identityHashHalfWordMask());
					if (/* is:outPointerClassHashFor:in:limit: */
						(((hash & TopHashBit) != 0))
					 && (((hash - TopHashBit) <= outIndex)
					 && (heapOop == (longAt((void *)((outPointerArray + BaseHeaderSize) + ((((usqInt)((hash - TopHashBit)) << (shiftForWord())))))))))) {
						oop = ((hash - TopHashBit) * BytesPerOop) + TopOopBit;
					}
					else {
						/* begin newOutPointer:at:in:hashes: */
						if (outIndex >= ((/* begin numSlotsOf: */
							assert((classIndexOf(outPointerArray)) > (isForwardedObjectClassIndexPun())),
						(((numSlotsUsqInt = byteAt((void *)(outPointerArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
									? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(outPointerArray - BaseHeaderSize)))) << 8)))))) >> 8
									: numSlotsUsqInt)))) {
							outIndex = 0;
							goto l3;
						}

						/* no room in outPointers; fail */

						/* begin storePointer:ofObject:withValue: */
						assert(validStorePointerArgs(outIndex, outPointerArray, heapOop));
						assert(isNonImmediate(outPointerArray));
						if (oopisGreaterThanOrEqualTo(outPointerArray, GIV(oldSpaceStart))) {
							if (/* isYoung: */
								((!(heapOop & (tagMask()))))
							 && (oopisLessThan(heapOop, GIV(oldSpaceStart)))) {
								/* begin possibleRootStoreInto: */
								if (!((byteAt((void *)(outPointerArray + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
									remember(outPointerArray);
								}
							}
						}

						/* most stores into young objects */
						longAtput((void *)((outPointerArray + BaseHeaderSize) + ((((usqInt)(outIndex) << (shiftForWord()))))),heapOop);
						valueWordSqInt = (long32At((void *)(heapOop + 4))) & (identityHashHalfWordMask());

						/* begin storeLong32:ofObject:withValue: */
						long32Atput((void *)((savedOutHashes + BaseHeaderSize) + ((((usqInt)(outIndex) << 2)))),valueWordSqInt);
						long32Atput((void *)(heapOop + 4),((((long32At((void *)(heapOop + 4))) | (identityHashHalfWordMask())) - (identityHashHalfWordMask()))) + ((outIndex + TopHashBit) & (identityHashHalfWordMask())));
						outIndex += 1;
						/* end newOutPointer:at:in:hashes: */
l3:
						if (!outIndex) {
							return -1;
						}

						/* no room in outPointers; fail */
						assert((((rawHashBitsOf(heapOop)) & TopHashBit) != 0));
						oop = ((((long32At((void *)(heapOop + 4))) & (identityHashHalfWordMask())) - TopHashBit) * BytesPerOop) + TopOopBit;
					}
				}

				/* oop is an object in the segment.
				   oop is an outPointer; locate or allocate its oop */

				/* storeUncheckedNoAssert:ofAny:withValue: */
				longAtput((void *)((objOop + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),oop);
			}
		}

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, segAddr)) {
			objOop = segAddr;
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l1:
		objIndex += 1;
	}
	return outIndex;
}
/* Extracted from interp.c:46933 (function compact). */

/*	Sweep all of old space, sliding unpinned marked objects down over free and
	unmarked objects.
	Let the segmentManager mark which segments contain pinned objects via
	notePinned:. 
 */

	/* SpurPlanningCompactor>>#compact */

static NeverInline void
compact(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt classIndex;
    sqInt finalPass;
    sqInt firstPass;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt highestSuitableFreeBlock;
    sqInt i;
    sqInt largestFreeChunk;
    sqInt largestFreeChunkSqInt;
    usqInt numSlots;
    sqInt objOopSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt sizeOfEden;
    sqInt spaceEstimate;
    sqInt stackOrNil;


	/* for profiling */
	/* begin initializeScanCheckingForFullyCompactedHeap */
	GIV(firstMobileObject) = (GIV(lastMobileObject) = (GIV(objectAfterLastMobileObject) = null));
	reinitializeScanFrom(GIV(hiddenRootsObj));
	if (!GIV(firstFreeObject)) {
		error("uncompactable heap; no unmarked objects found");
	}
	if (GIV(firstMobileObject) >= GIV(endOfMemory)) {
		/* begin unmarkObjectsInFullyCompactedHeap */
		/* begin unmarkInitialImmobileObjects */
		/* begin allOldSpaceObjectsFrom:do: */
		/* begin allOldSpaceEntitiesFrom:do: */
		assert(isOldObject(GIV(nilObj)));
		prevPrevObj = (prevObj = null);
		objOopSqInt = GIV(nilObj);
		while (1) {
			assert((objOopSqInt % (allocationUnit())) == 0);
			if (!(oopisLessThan(objOopSqInt, GIV(endOfMemory)))) break;
			assert((long64At((void *)(objOopSqInt))) != 0);

			/* begin isEnumerableObject: */
			classIndex = (longAt((void *)(objOopSqInt))) & (classIndexMask());
			assert((classIndex == (segmentBridgePun()))
			 || ((classIndex == (isForwardedObjectClassIndexPun()))
			 || (((long64At((void *)(objOopSqInt))) != 0)
			 && (classIndex < (GIV(numClassTablePages) * (classTablePageSize()))))));
			if (classIndex >= (isForwardedObjectClassIndexPun())) {
				if (oopisGreaterThanOrEqualTo(objOopSqInt, GIV(firstMobileObject))) {
					goto l2;
				}

				/* begin setIsMarkedOf:to: */
				assert(!(isFreeObject(objOopSqInt)));
				byteAtput((void *)(objOopSqInt + (markBitsByteOffset())),(byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
			}
			prevPrevObj = prevObj;
			prevObj = objOopSqInt;

			/* begin objectAfter:limit: */
			followingWordAddress = addressAfter(objOopSqInt);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
				objOopSqInt = GIV(endOfMemory);
				goto l4;
			}
			followingWord = longAt((void *)(followingWordAddress));
			objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
						? followingWordAddress + BaseHeaderSize
						: followingWordAddress);
			/* end objectAfter:limit: */
l4:;
		}
		/* end unmarkInitialImmobileObjects */
l2:

		/* begin unmarkSurvivingObjectsForCompact */
		/* begin allPastSpaceObjectsDo: */
		/* begin allPastSpaceEntitiesDo: */
		prevPrevObj = (prevObj = null);
		address = ((GIV(pastSpace)).start);

		/* begin objectStartingAt: */
		numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
		objOopSqInt = (numSlots == (numSlotsMask())
					? address + BaseHeaderSize
					: address);
		while (oopisLessThan(objOopSqInt, GIV(pastSpaceStart))) {
			assert(isEnumerableObjectNoAssert(objOopSqInt));
			if ((byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
				/* begin setIsMarkedOf:to: */
				assert(!(isFreeObject(objOopSqInt)));
				byteAtput((void *)(objOopSqInt + (markBitsByteOffset())),(byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
			}
			prevPrevObj = prevObj;
			prevObj = objOopSqInt;

			/* begin objectAfterMaybeSlimBridge:limit: */
			followingWordAddress = addressAfter(objOopSqInt);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(pastSpaceStart))) {
				objOopSqInt = GIV(pastSpaceStart);
				goto l3;
			}
			followingWord = longAt((void *)(followingWordAddress));
			objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
						? ((oopisLessThan(objOopSqInt, GIV(oldSpaceStart)))
						 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
								? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
								: followingWordAddress + BaseHeaderSize)
						: followingWordAddress);
			/* end objectAfterMaybeSlimBridge:limit: */
l3:;
		}
		return;
	}

	/* begin initializeCompaction */
	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) == (GCCheckFreeSpace | GCModeFull))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}

	/* begin selectSavedFirstFieldsSpace */
	spaceEstimate = (GIV(endOfMemory) - GIV(nilObj)) / 40;
	sizeOfEden = (((GIV(eden)).limit)) - (((GIV(eden)).start));
	if (spaceEstimate > sizeOfEden) {
		/* begin findHighestSuitableFreeBlock: */
		if ((largestFreeChunk = findLargestFreeChunk())) {
			if (((bytesInBody(largestFreeChunk)) >= spaceEstimate)
			 && ((((usqInt)largestFreeChunk)) > (((usqInt)(GIV(endOfMemory) - GIV(totalFreeOldSpace)))))) {
				highestSuitableFreeBlock = largestFreeChunk;
				goto l8;
			}
		}
		highestSuitableFreeBlock = null;
		/* end findHighestSuitableFreeBlock: */
l8:
		if (highestSuitableFreeBlock) {
			if ((spaceEstimate > ((/* begin sizeOfFree: */
				assert(isFreeObject(highestSuitableFreeBlock)),
			bytesInBody(highestSuitableFreeBlock))))
			 && (useSegmentForSavedFirstFieldsSpace(spaceEstimate))) {
				goto l5;
			}
			if (((/* begin sizeOfFree: */
				assert(isFreeObject(highestSuitableFreeBlock)),
			bytesInBody(highestSuitableFreeBlock))) > sizeOfEden) {
				/* begin useFreeChunkForSavedFirstFieldsSpace: */
				assert(validFreeTreeChunk(highestSuitableFreeBlock));
				(GIV(savedFirstFieldsSpace).start = highestSuitableFreeBlock + (4 /* freeChunkLargerIndex */ * BytesPerOop));
				(GIV(savedFirstFieldsSpace).limit = addressAfter(highestSuitableFreeBlock));
				GIV(savedFirstFieldsSpaceNotInOldSpace) = 0;
				assert(!((savedFirstFieldsSpaceWasAllocated())));
				goto l5;
			}
		}
		if (useSegmentForSavedFirstFieldsSpace(spaceEstimate)) {
			goto l5;
		}
	}

	/* begin useEdenForSavedFirstFieldsSpace */
	(GIV(savedFirstFieldsSpace).start = ((GIV(eden)).start));
	(GIV(savedFirstFieldsSpace).limit = ((GIV(eden)).limit));
	GIV(savedFirstFieldsSpaceNotInOldSpace) = 1;
	assert(!((savedFirstFieldsSpaceWasAllocated())));
	/* end selectSavedFirstFieldsSpace */
l5:

	/* begin unpinRememberedSet */
	/* begin fetchPointer:ofObject: */
	GIV(firstFieldOfRememberedSet) = longAt((void *)(((longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord()))))))) + BaseHeaderSize) + (0U << (shiftForWord()))));
	setIsPinnedOfto(longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord())))))), 0);

	/* begin resetFreeListHeads */
	GIV(freeListsMask) = 0;
	for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
		GIV(freeLists)[i] = 0;
	}

	/* begin prepareObjStacksForPlanningCompactor */
	/* begin prepareObjStackForPlanningCompactor: */
	if (GIV(markStack) == GIV(nilObj)) {
		goto l7;
	}
	stackOrNil = GIV(markStack);
	do {
		assert((numSlotsOfAny(stackOrNil)) == ObjStackPageSlots);

		/* begin setHashBitsOf:to: */
		long32Atput((void *)(stackOrNil + 4),((((long32At((void *)(stackOrNil + 4))) | (identityHashHalfWordMask())) - (identityHashHalfWordMask()))) + ((longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) & (identityHashHalfWordMask())));
	} while(((stackOrNil = longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord())))))))) != 0);
	/* end prepareObjStackForPlanningCompactor: */
l7:
	if (GIV(weaklingStack) == GIV(nilObj)) {
		goto l9;
	}
	stackOrNil = GIV(weaklingStack);
	do {
		assert((numSlotsOfAny(stackOrNil)) == ObjStackPageSlots);

		/* begin setHashBitsOf:to: */
		long32Atput((void *)(stackOrNil + 4),((((long32At((void *)(stackOrNil + 4))) | (identityHashHalfWordMask())) - (identityHashHalfWordMask()))) + ((longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) & (identityHashHalfWordMask())));
	} while(((stackOrNil = longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord())))))))) != 0);
	/* end prepareObjStackForPlanningCompactor: */
l9:
	if (GIV(mournQueue) == GIV(nilObj)) {
		goto l6;
	}
	stackOrNil = GIV(mournQueue);
	do {
		assert((numSlotsOfAny(stackOrNil)) == ObjStackPageSlots);

		/* begin setHashBitsOf:to: */
		long32Atput((void *)(stackOrNil + 4),((((long32At((void *)(stackOrNil + 4))) | (identityHashHalfWordMask())) - (identityHashHalfWordMask()))) + ((longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))))) & (identityHashHalfWordMask())));
	} while(((stackOrNil = longAt((void *)((stackOrNil + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord())))))))) != 0);
	/* end prepareObjStackForPlanningCompactor: */
l6:
	GIV(totalFreeOldSpace) = 0;
	GIV(gcPhaseInProgress) = SlidingCompactionInProgress;
	(GIV(savedFirstFieldsSpace).top = ((GIV(savedFirstFieldsSpace).start)) - BytesPerOop);
	firstPass = 1;
	while (1) {
		finalPass = planCompactSavingForwarders();
		assert((validRelocationPlanInPass(finalPass)) == 0);
		updatePointers();
		copyAndUnmark(firstPass);

		/* Would like to check here, but can't if multi-pass. */

		/* Currently we do only a single pass if a normal GC, assuming that a pass will
		   always compact plenty of space. But we should perhaps check this assumption
		   by looking at the large free tree and seeing that the ratio of the largest free
		   chunk to the total ammount of free space is high. */
		if (finalPass
		 || (GIV(biasForGC))) break;
		firstPass = 0;
		reinitializeScanFrom(GIV(firstFreeObject));
		if (!(GIV(firstMobileObject) > GIV(firstFreeObject))) {
			GIV(firstFreeObject) = GIV(firstMobileObject);
			unmarkObjectsFromFirstFreeObject();

			/* begin checkFreeSpace: */
			assert(bitsSetInFreeSpaceMaskForAllFreeLists());
			assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
			if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) == (GCCheckFreeSpace | GCModeFull))) {
				runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
			}
			endCompaction();
			return;
		}

		/* We get here when firstFreeObject is the last object in memory.
		   firstMobileObject may be random value from previous compaction
		   in this case, and heap is fully compacted. Abort compaction.
		   We set firstFreeObject to firstMobileObject for unmarking. */

		/* begin updateSavedFirstFieldsSpaceIfNecessary */
		if ((/* savedFirstFieldsSpaceInFreeChunk */
			(!GIV(savedFirstFieldsSpaceNotInOldSpace))
		 && (oopisGreaterThan((GIV(savedFirstFieldsSpace).start), GIV(nilObj))))
		 && (((GIV(savedFirstFieldsSpace).start)) < GIV(lastMobileObject))) {
			spaceEstimate = ((GIV(savedFirstFieldsSpace).limit)) - ((GIV(savedFirstFieldsSpace).start));

			/* begin findHighestSuitableFreeBlock: */
			if ((largestFreeChunkSqInt = findLargestFreeChunk())) {
				if (((bytesInBody(largestFreeChunkSqInt)) >= spaceEstimate)
				 && ((((usqInt)largestFreeChunkSqInt)) > (((usqInt)(GIV(endOfMemory) - GIV(totalFreeOldSpace)))))) {
					largestFreeChunk = largestFreeChunkSqInt;
					goto l1;
				}
			}
			largestFreeChunk = null;
			/* end findHighestSuitableFreeBlock: */
l1:
			if (largestFreeChunk) {
				/* begin useFreeChunkForSavedFirstFieldsSpace: */
				assert(validFreeTreeChunk(largestFreeChunk));
				(GIV(savedFirstFieldsSpace).start = largestFreeChunk + (4 /* freeChunkLargerIndex */ * BytesPerOop));
				(GIV(savedFirstFieldsSpace).limit = addressAfter(largestFreeChunk));
				GIV(savedFirstFieldsSpaceNotInOldSpace) = 0;
				assert(!((savedFirstFieldsSpaceWasAllocated())));
			}
			else {
				/* begin useEdenForSavedFirstFieldsSpace */
				(GIV(savedFirstFieldsSpace).start = ((GIV(eden)).start));
				(GIV(savedFirstFieldsSpace).limit = ((GIV(eden)).limit));
				GIV(savedFirstFieldsSpaceNotInOldSpace) = 1;
				assert(!((savedFirstFieldsSpaceWasAllocated())));
			}
		}
		(GIV(savedFirstFieldsSpace).top = ((GIV(savedFirstFieldsSpace).start)) - BytesPerOop);
	}

	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) == (GCCheckFreeSpace | GCModeFull))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}
	endCompaction();
}
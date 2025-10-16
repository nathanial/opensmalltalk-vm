/* Extracted from interp.c:41675 (function objectsReachableFromRoots). */

static NoDbgRegParms NeverInline sqInt
objectsReachableFromRoots(sqInt arrayOfRoots)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt count;
    sqInt delta;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt freeChunk;
    sqInt i;
    sqInt iSqInt;
    usqInt limit;
    usqInt numSlots;
    sqInt objOop;
    sqInt objOopSqInt;
    sqInt oop;
    sqInt oopSqInt;
    sqInt prevObj;
    sqInt prevPrevObj;
    usqInt ptr;
    sqInt referent;
    usqInt start;
    usqInt startUsqInt;
    sqInt toDoLimit;
    sqInt toDoLimit1;
    sqInt toDoLimitSqInt;


	/* & hence start & ptr are too; limit is also because of addressAfter: */
	assert(isArray(arrayOfRoots));

	/* Mark all objects except those only reachable from the arrayOfRoots by marking
	   each object in arrayOfRoots and then marking all reachable objects (from the
	   system roots).  This leaves unmarked only objects reachable from the arrayOfRoots.
	   N.B. A side-effect of the marking is that all forwarders in arrayOfRoots will be followed. */
	assert(allObjectsUnmarked());

	/* setGCMode: */

	/* begin markObjectsIn: */
	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(arrayOfRoots)));
	byteAtput((void *)(arrayOfRoots + (markBitsByteOffset())),(byteAt((void *)(arrayOfRoots + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
	toDoLimitSqInt = ((/* begin numSlotsOf: */
	assert((classIndexOf(arrayOfRoots)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(arrayOfRoots + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(arrayOfRoots - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
	for (iSqInt = 0; iSqInt <= toDoLimitSqInt; iSqInt += 1) {
		/* begin followField:ofObject: */
		oopSqInt = longAt((void *)((arrayOfRoots + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))));
		if (/* isOopForwarded: */
			((!(oopSqInt & (tagMask()))))
		 && ((!((longAt((void *)(oopSqInt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			oopSqInt = fixFollowedFieldofObjectwithInitialValue(iSqInt, arrayOfRoots, oopSqInt);
		}
		if ((!(oopSqInt & (tagMask())))) {
			/* begin setIsMarkedOf:to: */
			assert(!(isFreeObject(oopSqInt)));
			byteAtput((void *)(oopSqInt + (markBitsByteOffset())),(byteAt((void *)(oopSqInt + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
		}
	}
	markObjects(0);

	/* After the mark phase all unreachable weak slots will have been nilled
	   and all active ephemerons fired. */
	assert(isEmptyObjStack(GIV(markStack)));
	assert(isEmptyObjStack(GIV(weaklingStack)));
	assert(noUnscannedEphemerons());

	/* Now unmark the roots before collecting the transitive closure of unmarked objects accessible from the roots. */

	/* begin unmarkObjectsIn: */
	toDoLimitSqInt = ((assert((classIndexOf(arrayOfRoots)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(arrayOfRoots + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(arrayOfRoots - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
	for (iSqInt = 0; iSqInt <= toDoLimitSqInt; iSqInt += 1) {
		/* begin followField:ofObject: */
		oopSqInt = longAt((void *)((arrayOfRoots + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))));
		if (/* isOopForwarded: */
			((!(oopSqInt & (tagMask()))))
		 && ((!((longAt((void *)(oopSqInt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			oopSqInt = fixFollowedFieldofObjectwithInitialValue(iSqInt, arrayOfRoots, oopSqInt);
		}
		if ((!(oopSqInt & (tagMask())))) {
			/* begin setIsMarkedOf:to: */
			assert(!(isFreeObject(oopSqInt)));
			byteAtput((void *)(oopSqInt + (markBitsByteOffset())),(byteAt((void *)(oopSqInt + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
		}
	}

	/* Use the largest free chunk to answer the result. */

	/* N.B. Does /not/ update totalFreeOldSpace */
	freeChunk = allocateLargestFreeChunk();

	/* but must update so that growth in the markStack does not cause assert fails. */
	GIV(totalFreeOldSpace) -= bytesInBody(freeChunk);
	ptr = (start = freeChunk + BaseHeaderSize);
	limit = addressAfter(freeChunk);
	count = 0;

	/* First put the arrayOfRoots; order is important. */
	noCheckPushonObjStack(arrayOfRoots, GIV(markStack));

	/* Now collect the roots and the transitive closure of unmarked objects from them. */
	while (!(isEmptyObjStack(GIV(markStack)))) {
		objOop = popObjStack(GIV(markStack));
		assert(isMarked(objOop));
		count += 1;
		if (ptr < limit) {
			longAtput((void *)(ptr),objOop);
			ptr += BytesPerOop;
		}
		oop = fetchClassOfNonImm(objOop);
		if (!((byteAt((void *)(oop + (markBitsByteOffset())))) & (1U << (markedBitByteShift())))) {
			/* begin setIsMarkedOf:to: */
			assert(!(isFreeObject(oop)));
			byteAtput((void *)(oop + (markBitsByteOffset())),(byteAt((void *)(oop + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
			noCheckPushonObjStack(oop, GIV(markStack));
		}
		if ((((longAt((void *)(objOop))) & (classIndexMask())) == ClassMethodContextCompactIndex)
		 && (/* isStillMarriedContext: */
			(((((longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(objOop))))) {
			toDoLimit = (numSlotsOfMarriedContext(objOop)) - 1;
			for (i = 0; i <= toDoLimit; i += 1) {
				oop = fetchPointerofMarriedContext(i, objOop);
				if (!((((oop & (tagMask())) != 0))
					 || (((byteAt((void *)(oop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))) {
					/* begin setIsMarkedOf:to: */
					assert(!(isFreeObject(oop)));
					byteAtput((void *)(oop + (markBitsByteOffset())),(byteAt((void *)(oop + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
					noCheckPushonObjStack(oop, GIV(markStack));
				}
			}
		}
		else {
			toDoLimit1 = (numPointerSlotsOf(objOop)) - 1;
			for (i = 0; i <= toDoLimit1; i += 1) {
				oop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
				if (!((((oop & (tagMask())) != 0))
					 || (((byteAt((void *)(oop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))) {
					/* begin setIsMarkedOf:to: */
					assert(!(isFreeObject(oop)));
					byteAtput((void *)(oop + (markBitsByteOffset())),(byteAt((void *)(oop + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
					noCheckPushonObjStack(oop, GIV(markStack));
				}
			}
		}
	}

	/* begin unmarkAllObjects */
	/* begin allHeapEntitiesDo: */
	/* begin allOldSpaceEntitiesDo: */
	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(GIV(nilObj)));
	prevPrevObj = (prevObj = null);
	objOopSqInt = GIV(nilObj);
	while (1) {
		assert((objOopSqInt % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOopSqInt, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objOopSqInt))) != 0);
		if ((byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
			if (((longAt((void *)(objOopSqInt))) & (classIndexMask())) > (lastClassIndexPun())) {
				/* begin setIsMarkedOf:to: */
				assert(!(isFreeObject(objOopSqInt)));
				byteAtput((void *)(objOopSqInt + (markBitsByteOffset())),(byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
			}
			else {
				if (!(((longAt((void *)(objOopSqInt))) & (classIndexMask())) == (segmentBridgePun()))) {
					/* begin setIsMarkedOf:to: */
					assert(!(isFreeObject(objOopSqInt)));
					byteAtput((void *)(objOopSqInt + (markBitsByteOffset())),(byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
				}
			}
		}
		prevPrevObj = prevObj;
		prevObj = objOopSqInt;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOopSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			objOopSqInt = GIV(endOfMemory);
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l1:;
	}

	/* begin allNewSpaceEntitiesDo: */
	prevPrevObj = (prevObj = null);

	/* After a scavenge eden is empty, futureSpace is empty, and all newSpace objects are
	   in pastSpace.  Objects are allocated in eden.  So enumerate only pastSpace and eden. */
	assert((((GIV(pastSpace)).start)) < (((GIV(eden)).start)));
	startUsqInt = /* startAddressForBridgedHeapEnumeration */
			(GIV(pastSpaceStart) > (((GIV(pastSpace)).start))
				? ((GIV(pastSpace)).start)
				: (GIV(freeStart) > (((GIV(eden)).start))
						? ((GIV(eden)).start)
						: GIV(oldSpaceStart)));
	if (startUsqInt > GIV(freeStart)) {
		goto l3;
	}

	/* begin bridgePastSpaceAndEden */
	if (GIV(pastSpaceStart) < (((GIV(eden)).start))) {
		if ((GIV(pastSpaceStart) + BaseHeaderSize) == (((GIV(eden)).start))) {
			hackSlimBridgeToat(objectStartingAt(((GIV(eden)).start)), GIV(pastSpaceStart));

			/* And carefully check the assumption */
			assert((objectAfterMaybeSlimBridgelimit(objectInPastSpaceBefore(GIV(pastSpaceStart)), GIV(nilObj))) == (objectStartingAt(((GIV(eden)).start))));
		}
		else {
			initSegmentBridgeWithBytesat((((GIV(eden)).start)) - GIV(pastSpaceStart), GIV(pastSpaceStart));
		}
	}

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(startUsqInt + (numSlotsFieldByteOffset())));
	objOopSqInt = (numSlots == (numSlotsMask())
				? startUsqInt + BaseHeaderSize
				: startUsqInt);
	while (oopisLessThan(objOopSqInt, GIV(freeStart))) {
		if ((byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
			if (((longAt((void *)(objOopSqInt))) & (classIndexMask())) > (lastClassIndexPun())) {
				/* begin setIsMarkedOf:to: */
				assert(!(isFreeObject(objOopSqInt)));
				byteAtput((void *)(objOopSqInt + (markBitsByteOffset())),(byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
			}
			else {
				if (!(((longAt((void *)(objOopSqInt))) & (classIndexMask())) == (segmentBridgePun()))) {
					/* begin setIsMarkedOf:to: */
					assert(!(isFreeObject(objOopSqInt)));
					byteAtput((void *)(objOopSqInt + (markBitsByteOffset())),(byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
				}
			}
		}
		prevPrevObj = prevObj;
		prevObj = objOopSqInt;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOopSqInt);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(freeStart))) {
			objOopSqInt = GIV(freeStart);
			goto l2;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOopSqInt, GIV(oldSpaceStart)))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l2:;
	}
	/* end allNewSpaceEntitiesDo: */
l3:

	/* Now try and allocate the result */
	if ((count > ((ptr - start) / BytesPerOop))
	 || ((limit != ptr)
	 && ((limit - ptr) <= 8 /* allocationUnit */))) {
		freeObject(freeChunk);

		/* begin checkFreeSpace: */
		assert(bitsSetInFreeSpaceMaskForAllFreeLists());
		assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
		if (((checkForLeaks & (GCCheckFreeSpace | GCModeImageSegment)) == (GCCheckFreeSpace | GCModeImageSegment))) {
			runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
		}

		/* setGCMode: */
		return (((usqInt)count << 3) | 1);
	}

	/* not enough room
	   can't split a single word
	   There's room; set the format, & classIndex and shorten. */

	/* begin setFormatOf:to: */
	assert((((arrayFormat()) >= 0) && ((arrayFormat()) <= (formatMask()))));
	byteAtput((void *)(freeChunk + (formatFieldByteOffset())),((byteAt((void *)(freeChunk + (formatFieldByteOffset())))) & (0xFF - (formatMask()))) + (arrayFormat()));

	/* begin setClassIndexOf:to: */
	assert(((ClassArrayCompactIndex >= 0) && (ClassArrayCompactIndex <= (classIndexMask()))));
	longAtput((void *)(freeChunk),((longAt((void *)(freeChunk))) & (~(usqIntptr_t)(classIndexMask()))) + ClassArrayCompactIndex);

	/* begin shorten:toIndexableSize: */
	assert((count >= 0)
	 && (count < (lengthOf(freeChunk))));
	delta = doShortentoIndexableSize(freeChunk, count);
	assert((lengthOf(followMaybeForwarded(freeChunk))) == count);

	/* Don't check if checking image segments, because we will check immediately after
	   the shorten in storeImageSegmentInto:outPointers:roots: */
	if (!(((checkForLeaks & GCModeImageSegment) != 0))) {
		runLeakCheckerFor(GCCheckShorten);
	}
	if ((!((longAt((void *)(freeChunk))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		/* begin followForwarded: */
		assert(isUnambiguouslyForwarder(freeChunk));
		referent = longAt((void *)((freeChunk + BaseHeaderSize) + (0U << (shiftForWord()))));
		while (/* isOopForwarded: */
			((!(referent & (tagMask()))))
		 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
		}
		freeChunk = referent;
	}

	/* begin possibleRootStoreInto: */
	if (!((byteAt((void *)(freeChunk + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
		remember(freeChunk);
	}

	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCModeImageSegment)) == (GCCheckFreeSpace | GCModeImageSegment))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}
	runLeakCheckerFor(GCModeImageSegment);

	/* setGCMode: */
	return freeChunk;
}
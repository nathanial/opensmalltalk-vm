/* Extracted from interp.c:45764 (function storeImageSegmentIntooutPointersroots). */

static NoDbgRegParms sqInt
storeImageSegmentIntooutPointersroots(sqInt segmentWordArrayArg, sqInt outPointersArrayArg, sqInt arrayOfRootsArg)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arrayOfObjects;
    sqInt arrayOfRoots;
    sqInt delta;
    usqInt endSeg;
    sqInt hash;
    sqInt here;
    sqInt i;
    sqInt indexableSize;
    sqInt newSegAddrOrError;
    sqInt numClassesInSegment;
    usqInt numSlots;
    sqInt numSlotsSqInt;
    sqInt objOop;
    sqInt objOopSqInt;
    sqInt outIndex;
    sqInt outPointersArray;
    usqInt p;
    sqInt savedFirstFields;
    sqInt savedOutHashes;
    usqInt segAddr;
    sqInt segmentWordArray;
    usqInt segStart;
    sqInt tempObjOop;
    sqInt there;
    sqInt toDoLimit;
    sqInt toDoLimitSqInt;
    usqInt toDoLimitUsqInt;

	if ((
#  if IMMUTABILITY
		((((usqInt)((byteAt((void *)(segmentWordArrayArg + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0
#  else // IMMUTABILITY
		0
#  endif
		)
	 || (
#  if IMMUTABILITY
		((((usqInt)((byteAt((void *)(outPointersArrayArg + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0
#  else // IMMUTABILITY
		0
#  endif
		)) {
		return PrimErrNoModification;
	}

	/* Since segmentWordArrayArg & outPointersArrayArg may get shortened, they can't be pinned. */
	if ((((byteAt((void *)(segmentWordArrayArg + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)
	 || (((byteAt((void *)(outPointersArrayArg + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)) {
		return PrimErrObjectIsPinned;
	}
	if (((/* begin numSlotsOf: */
		assert((classIndexOf(outPointersArrayArg)) > (isForwardedObjectClassIndexPun())),
	(((numSlots = byteAt((void *)(outPointersArrayArg + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(outPointersArrayArg - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots))) > (identityHashHalfWordMask())) {
		return PrimErrLimitExceeded;
	}
	runLeakCheckerFor(GCModeImageSegment);

	/* First scavenge to collect any new space garbage that refers to the graph. */

	/* begin scavengingGC */
	scavengingGCTenuringIf(TenureByAge);
	segmentWordArray = /* updatePostScavenge: */
			((!((longAt((void *)(segmentWordArrayArg))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
				? followForwarded(segmentWordArrayArg)
				: segmentWordArrayArg);
	outPointersArray = /* updatePostScavenge: */
			((!((longAt((void *)(outPointersArrayArg))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
				? followForwarded(outPointersArrayArg)
				: outPointersArrayArg);
	arrayOfRoots = /* updatePostScavenge: */
			((!((longAt((void *)(arrayOfRootsArg))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
				? followForwarded(arrayOfRootsArg)
				: arrayOfRootsArg);
	assert(!((forwardersIn(outPointersArray))));
	assert(!((forwardersIn(arrayOfRoots))));

	/* Now compute the transitive closure, collecting the sequence of objects to be stored in the arrayOfObjects array.
	   Included in arrayOfObjects are the arrayOfRoots and all its contents.  All objects have been unmarked. */
	arrayOfObjects = objectsReachableFromRoots(arrayOfRoots);
	if (!arrayOfObjects) {
		return PrimErrNoMemory;
	}

	/* If objectsReachableFromRoots: answers an integer there is not enough continuous free space in which to allocate the
	   reachable objects.  If there is sufficient free space then answer an error code to prompt a compacting GC and a retry. */
	if ((((arrayOfObjects) & 7) == 1)) {
		if ((GIV(totalFreeOldSpace) - 8 /* allocationUnit */) >= ((arrayOfObjects >> 3))) {
			return PrimErrNeedCompaction;
		}
		return PrimErrNoMemory;
	}
	assert(allObjectsUnmarked());
	assert(!((forwardersIn(arrayOfObjects))));
	runLeakCheckerFor(GCModeImageSegment);

	/* Both to expand the max size of segment and to reduce the length of the
	   load-time pass that adds classes to the class table, move classes to the
	   front of arrayOfObjects, leaving the root array as the first element. */

	/* begin moveClassesForwardsIn: */
	/* if > 0, this is the index of the first non-class past the first element. */
	numClassesInSegment = (there = 0);
	toDoLimitSqInt = ((/* begin numSlotsOf: */
	assert((classIndexOf(arrayOfObjects)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(arrayOfObjects + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(arrayOfObjects - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
	for (here = 1; here <= toDoLimitSqInt; here += 1) {
		objOopSqInt = longAt((void *)((arrayOfObjects + BaseHeaderSize) + ((((usqInt)(here) << (shiftForWord()))))));
		hash = (long32At((void *)(objOopSqInt + 4))) & (identityHashHalfWordMask());
		if ((hash > (lastClassIndexPun()))
		 && ((classOrNilAtIndex(hash)) == objOopSqInt)) {
			numClassesInSegment += 1;
			if (there > 0) {
				tempObjOop = longAt((void *)((arrayOfObjects + BaseHeaderSize) + ((((usqInt)(there) << (shiftForWord()))))));

				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(arrayOfObjects))
				 && (!(isForwarded(arrayOfObjects))));
				assert(validStorePointerUncheckedArgs(there, arrayOfObjects, objOopSqInt));
				longAtput((void *)((arrayOfObjects + BaseHeaderSize) + ((((usqInt)(there) << (shiftForWord()))))),objOopSqInt);

				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(arrayOfObjects))
				 && (!(isForwarded(arrayOfObjects))));
				assert(validStorePointerUncheckedArgs(here, arrayOfObjects, tempObjOop));
				longAtput((void *)((arrayOfObjects + BaseHeaderSize) + ((((usqInt)(here) << (shiftForWord()))))),tempObjOop);
				there += 1;
			}
		}
		else {
			if (!there) {
				there = here;
			}
		}
	}

	/* The scheme is to copy the objects into segmentWordArray, and then map the oops in segmentWordArray.
	   Therefore the primitive needs to both map efficiently originals to copies in segmentWordArray and
	   be able to undo any side-effects if the primitive fails because either segmentWordArray or outPointersArray
	   is too small.  The mapping is done by having the objects to be stored in arrayOfObjects refer to their mapped
	   locations through their first field, just like a forwarding pointer, but without becoming a forwarder, saving their
	   first field in savedFirstFields, and the objects in outPointersArray pointing to their locations in the outPointersArray
	   through their identityHashes, saved in savedOutHashes.
	   Since arrayOfObjects and its savedFirstFields, and outPointersArray and its saved hashes, can be enumerated
	   side-by-side, the hashes can be restored to the originals.  So the first field of the heap object corresponding to
	   an object in arrayOfObjects is set to its location in segmentWordArray, and the hash of an object in outPointersArray
	   is set to its index in outPointersArray plus the top hash bit.  Classes in arrayOfObjects have their marked bit set.
	   Oops in objects in segmentWordArray are therefore mapped by accessing the original oop, and following its first
	   field. Class indices in segmentWordArray are mapped by fetching the original class, and testing its marked bit.
	   If marked, the first field is followed to access the class copy in the segment.  Out pointers (objects and classes,
	   which are unmarked), the object's identityHash is set (eek!!) to its index in the outPointersArray. So savedOutHashes
	   parallels the outPointersArray. The saved hash array is initialized with an out-of-range hash value so that the first
	   unused entry can be identified. */
	savedFirstFields = noInlineAllocateSlotsformatclassIndex(numSlotsOf(arrayOfObjects), wordIndexableFormat(), wordSizeClassIndexPun());
	savedOutHashes = noInlineAllocateSlotsformatclassIndex(numSlotsForBytes((numSlotsOf(outPointersArray)) * 4), firstLongFormat(), thirtyTwoBitLongsClassIndexPun());
	if ((!savedFirstFields)
	 || (!savedOutHashes)) {
		freeObject(arrayOfObjects);
		if (oopisGreaterThanOrEqualToandLessThan(savedFirstFields, GIV(oldSpaceStart), GIV(endOfMemory))) {
			freeObject(savedFirstFields);
		}
		if (oopisGreaterThanOrEqualToandLessThan(savedOutHashes, GIV(oldSpaceStart), GIV(endOfMemory))) {
			freeObject(savedOutHashes);
		}
		return PrimErrNoMemory;
	}
	/* begin numSlotsOf: */
	assert((classIndexOf(savedFirstFields)) > (isForwardedObjectClassIndexPun()));
	if (((numSlots = byteAt((void *)(savedFirstFields + (numSlotsFieldByteOffset()))))) == (numSlotsMask())) {
		numSlotsSqInt = ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(savedFirstFields - BaseHeaderSize)))) << 8)))))) >> 8;
	}
	else {
		numSlotsSqInt = numSlots;
	}

	/* begin fillObj:numSlots:with: */
	assert(oopisLessThan(((savedFirstFields + BaseHeaderSize) + (numSlotsSqInt * BytesPerOop)) - 1, addressAfter(savedFirstFields)));
	toDoLimitUsqInt = ((usqInt)(((savedFirstFields + BaseHeaderSize) + (numSlotsSqInt * BytesPerOop)) - 1));
	for (p = (((usqInt)(savedFirstFields + BaseHeaderSize))); p <= toDoLimitUsqInt; p += 8 /* allocationUnit */) {
		longAtput((void *)(p),0);
	}
	/* begin numSlotsOf: */
	assert((classIndexOf(savedOutHashes)) > (isForwardedObjectClassIndexPun()));
	if (((numSlots = byteAt((void *)(savedOutHashes + (numSlotsFieldByteOffset()))))) == (numSlotsMask())) {
		numSlotsSqInt = ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(savedOutHashes - BaseHeaderSize)))) << 8)))))) >> 8;
	}
	else {
		numSlotsSqInt = numSlots;
	}

	/* begin fillObj:numSlots:with: */
	assert(oopisLessThan(((savedOutHashes + BaseHeaderSize) + (numSlotsSqInt * BytesPerOop)) - 1, addressAfter(savedOutHashes)));
	toDoLimitUsqInt = ((usqInt)(((savedOutHashes + BaseHeaderSize) + (numSlotsSqInt * BytesPerOop)) - 1));
	for (p = (((usqInt)(savedOutHashes + BaseHeaderSize))); p <= toDoLimitUsqInt; p += 8 /* allocationUnit */) {
		longAtput((void *)(p),((((usqInt)(((identityHashHalfWordMask()) + 1)) << 32))) + ((identityHashHalfWordMask()) + 1));
	}
	segAddr = segmentWordArray + BaseHeaderSize;
	endSeg = addressAfter(segmentWordArray);

	/* Write a version number for byte order and version check. */
	if (segAddr >= endSeg) {
		return PrimErrGenericFailure;
	}
	long32Atput((void *)(segAddr),imageSegmentVersion());
	long32Atput((void *)(segAddr + 4),imageSegmentVersion());
	segStart = (segAddr += 8 /* allocationUnit */);
	assert(arrayOfRoots == (fetchPointerofObject(0, arrayOfObjects)));

	/* Copy all reachable objects to the segment, setting the marked bit for all objects (clones) in the segment,
	   and the remembered bit for all classes (clones) in the segment. */
	toDoLimit = ((/* begin numSlotsOf: */
	assert((classIndexOf(arrayOfObjects)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(arrayOfObjects + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(arrayOfObjects - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - 1;
	for (i = 0; i <= toDoLimit; i += 1) {
		if ((i == numClassesInSegment)
		 && ((((segAddr - segStart) / 8 /* allocationUnit */) + (lastClassIndexPun())) >= TopHashBit)) {
			return returnrestoringObjectsInupTosavedFirstFields(PrimErrLimitExceeded, arrayOfObjects, i, savedFirstFields);
		}
		objOop = longAt((void *)((arrayOfObjects + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		assert(!(((isImmediate(objOop))
 || (isForwarded(objOop)))));
		newSegAddrOrError = copyObjtoAddrstopAtsavedFirstFieldsindex(objOop, segAddr, endSeg, savedFirstFields, i);
		if (oopisLessThan(newSegAddrOrError, segStart)) {
			return returnrestoringObjectsInupTosavedFirstFields(newSegAddrOrError, arrayOfObjects, i, savedFirstFields);
		}
		segAddr = newSegAddrOrError;
	}

	/* Check that classes in the segment are addressable.  Since the top bit of the hash field is used to tag
	   classes external to the segment, the segment offset must not inadvertently set this bit.  This limit still
	   allows for a million or more classes.
	   Check that it can be safely shortened. */
	if ((endSeg != segAddr)
	 && ((endSeg - segAddr) < (BaseHeaderSize + BytesPerOop))) {
		return returnrestoringObjectsInupTosavedFirstFields(PrimErrWritePastObject, arrayOfObjects, -1, savedFirstFields);
	}

	/* Now scan, adding out pointers to the outPointersArray; all objects in arrayOfObjects
	   have their first field pointing to the corresponding copy in segmentWordArray. */
	if (((outIndex = mapOopsFromtooutPointersoutHashes(segStart, segAddr, outPointersArray, savedOutHashes))) < 0) {
		return returnrestoringObjectsInsavedFirstFieldsandsavedHashes(PrimErrBadIndex, arrayOfObjects, savedFirstFields, outPointersArray, savedOutHashes);
	}

	/* no room in outPointers; fail
	   We're done.  Shorten the results, restore hashes and return. */
	indexableSize = (segAddr - (segmentWordArray + BaseHeaderSize)) / 4;

	/* begin shorten:toIndexableSize: */
	assert((indexableSize >= 0)
	 && (indexableSize < (lengthOf(segmentWordArray))));
	delta = doShortentoIndexableSize(segmentWordArray, indexableSize);
	assert((lengthOf(followMaybeForwarded(segmentWordArray))) == indexableSize);

	/* Don't check if checking image segments, because we will check immediately after
	   the shorten in storeImageSegmentInto:outPointers:roots: */
	if (!(((checkForLeaks & GCModeImageSegment) != 0))) {
		runLeakCheckerFor(GCCheckShorten);
	}

	/* begin shorten:toIndexableSize: */
	assert((outIndex >= 0)
	 && (outIndex < (lengthOf(outPointersArray))));
	delta = doShortentoIndexableSize(outPointersArray, outIndex);
	assert((lengthOf(followMaybeForwarded(outPointersArray))) == outIndex);

	/* Don't check if checking image segments, because we will check immediately after
	   the shorten in storeImageSegmentInto:outPointers:roots: */
	if (!(((checkForLeaks & GCModeImageSegment) != 0))) {
		runLeakCheckerFor(GCCheckShorten);
	}
	return returnrestoringObjectsInsavedFirstFieldsandsavedHashes(PrimNoErr, arrayOfObjects, savedFirstFields, outPointersArray, savedOutHashes);
}
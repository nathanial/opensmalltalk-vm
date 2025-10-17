/* Extracted from interp.c:36486 (function initializeObjectMemory). */

/*	Initialize object memory variables at startup time. Assume endOfMemory at
	al are
	initialised by the image-reading code via
	setHeapBase:memoryLimit:endOfMemory:. endOfMemory is assumed to point to
	the end of the last object in the image.
	Assume: image reader also initializes the following variables:
	specialObjectsOop
	lastHash */
/*	Catch mis-initializations leading to bad translations to C */

	/* SpurMemoryManager>>#initializeObjectMemory: */

static NoDbgRegParms void
initializeObjectMemory(sqInt bytesToShift)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt anOop;
    sqInt cameFrom;
    usqInt chunkBytes;
    sqInt classIndex;
    sqInt classTableRoot;
    sqInt field;
    sqInt fieldAddr;
    sqInt fieldOop;
    sqInt firstObj;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt freeChunk;
    sqInt freeListObj;
    usqInt freeOldStart;
    sqInt fudge;
    sqInt i;
    sqInt index;
    sqInt j;
    sqInt largeChild;
    usqInt limit;
    sqInt min;
    sqInt nilObjPreSwizzle;
    usqInt numSlots;
    sqInt obj;
    usqInt p;
    sqInt page;
    sqInt smallChild;
    usqInt startOfFreeOldSpace;
    usqInt toDoLimit;
    sqInt treeNode;
    sqInt valuePointer;

	assert(BaseHeaderSize == BaseHeaderSize);
	assert((((sqInt)((maxSlotsForAlloc()) * BytesPerWord))) > 0);
	initSegmentBridgeWithBytesat(2 * BaseHeaderSize, GIV(endOfMemory) - (2 * BaseHeaderSize));
	adjustSegmentSwizzlesBy(bytesToShift);

	/* image may be at a different address; adjust oops for new location */

	/* begin adjustAllOopsBy: */
	assert(newSpaceIsEmpty());
	numSlots = byteAt((void *)(GIV(oldSpaceStart) + (numSlotsFieldByteOffset())));
	firstObj = (numSlots == (numSlotsMask())
				? GIV(oldSpaceStart) + BaseHeaderSize
				: GIV(oldSpaceStart));
	classTableRoot = oldSpaceObjectAfter(oldSpaceObjectAfter(oldSpaceObjectAfter(oldSpaceObjectAfter(firstObj))));
	nilObjPreSwizzle = GIV(oldSpaceStart) - bytesToShift;

	/* begin numSlotsOf: */
	assert((classIndexOf(classTableRoot)) > (isForwardedObjectClassIndexPun()));
	GIV(numClassTablePages) = (((numSlots = byteAt((void *)(classTableRoot + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(classTableRoot - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	assert(GIV(numClassTablePages) == ((classTableRootSlots()) + (hiddenRootSlots())));
	for (i = 2; i < GIV(numClassTablePages); i += 1) {
		if ((longAt((void *)((classTableRoot + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))))) == nilObjPreSwizzle) {
			GIV(numClassTablePages) = i;
			goto l4;
		}
	}
	/* end countNumClassPagesPreSwizzle: */
l4:
	if ((bytesToShift != 0)
	 || (GIV(numSegments) > 1)) {
		/* begin objectStartingAt: */
		numSlots = byteAt((void *)(GIV(oldSpaceStart) + (numSlotsFieldByteOffset())));
		obj = (numSlots == (numSlotsMask())
					? GIV(oldSpaceStart) + BaseHeaderSize
					: GIV(oldSpaceStart));
		while (oopisLessThan(obj, GIV(freeOldSpaceStart))) {
			classIndex = (longAt((void *)(obj))) & (classIndexMask());
			if (classIndex >= (isForwardedObjectClassIndexPun())) {
				/* begin swizzleFieldsOfObject: */
				fieldAddr = obj + (lastPointerOfWhileSwizzling(obj));
				while (oopisGreaterThanOrEqualTo(fieldAddr, obj + BaseHeaderSize)) {
					fieldOop = longAt((void *)(fieldAddr));
					if ((!(fieldOop & (tagMask())))) {
						longAtput((void *)(fieldAddr),swizzleObj(fieldOop));
					}
					fieldAddr -= BytesPerOop;
				}
			}
			else {
				if (classIndex == (isFreeObjectClassIndexPun())) {
					/* begin swizzleFieldsOfFreeChunk: */
					field = longAt((void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))));
					if (field) {
						valuePointer = swizzleObj(field);

						/* begin storeUncheckedNoAssert:ofAny:withValue: */
						longAtput((void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))),valuePointer);
					}
					chunkBytes = bytesInBody(obj);

					/* The prevPointer is not guaranteed to be valid in older images.
					   updateListStartingAt: via updateFreeLists does restore the prev pointer
					   in all small free lists, so simply avoid swizzling it now. */
					if (chunkBytes >= 0x200 /* (numFreeLists * #allocationUnit) */) {
						for (index = 2 /* freeChunkParentIndex */; index <= 4 /* freeChunkLargerIndex */; index += 1) {
							field = longAt((void *)((obj + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
							if (field) {
								valuePointer = swizzleObj(field);

								/* begin storeUncheckedNoAssert:ofAny:withValue: */
								longAtput((void *)((obj + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))),valuePointer);
							}
						}
					}
				}
			}
			followingWordAddress = addressAfter(obj);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
				obj = GIV(endOfMemory);
				goto l3;
			}
			followingWord = longAt((void *)(followingWordAddress));
			obj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
						? followingWordAddress + BaseHeaderSize
						: followingWordAddress);
			/* end objectAfter:limit: */
l3:;
		}
	}
	GIV(specialObjectsOop) = swizzleObj(GIV(specialObjectsOop));

	/* heavily used special objects */
	GIV(nilObj) = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(NilObject) << (shiftForWord()))))));
	GIV(falseObj) = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(FalseObject) << (shiftForWord()))))));
	GIV(trueObj) = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(TrueObject) << (shiftForWord()))))));

	/* In Cog we insist that nil, true & false are next to each other (Cogit generates tighter
	   conditional branch code as a result).  In addition, Spur places the free lists and
	   class table root page immediately following them. */
	assert(GIV(nilObj) == GIV(oldSpaceStart));
	assert(GIV(falseObj) == (oldSpaceObjectAfter(GIV(nilObj))));
	assert(GIV(trueObj) == (oldSpaceObjectAfter(GIV(falseObj))));
	freeListObj = oldSpaceObjectAfter(GIV(trueObj));
	anOop = oldSpaceObjectAfter(freeListObj);

	/* begin setHiddenRootsObj: */
	GIV(hiddenRootsObj) = anOop;
	assert(validClassTableRootPages());
	GIV(classTableFirstPage) = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + (0U << (shiftForWord()))));
	assert(((numSlotsOf(GIV(classTableFirstPage))) - 1) == (classTableMinorIndexMask()));

	/* Hack fix.  A bug in markAndTraceClassOf: caused the class of the first class table page
	   to be changed from its pun.  This can be restored manually, but we do it here too. */
	if (((longAt((void *)(GIV(classTableFirstPage)))) & (classIndexMask())) != (arrayClassIndexPun())) {
		/* begin setClassIndexOf:to: */
		assert((((arrayClassIndexPun()) >= 0) && ((arrayClassIndexPun()) <= (classIndexMask()))));
		longAtput((void *)(GIV(classTableFirstPage)),((longAt((void *)(GIV(classTableFirstPage)))) & (~(usqIntptr_t)(classIndexMask()))) + (arrayClassIndexPun()));
	}
	GIV(numClassTablePages) = 1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift()));

	/* Set classTableIndex to the start of the last used page (excepting first page).
	   Set numClassTablePages to the number of used pages.
	   If loading an image, set the classTableIndex to the first unused slot in the class table after the first page.
	   Set numClassTablePages to the number of used pages.
	   Set classTableIndex to point at the first unused entry. First set it to the max as a sentinel. */
	GIV(classTableIndex) = ((sqInt)((usqInt)(GIV(numClassTablePages)) << (classTableMajorIndexShift())));
	for (i = 1; i < GIV(numClassTablePages); i += 1) {
		if (((page = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord())))))))) == GIV(nilObj)) {
			if ((((usqInt)(GIV(classTableIndex))) >> (classTableMajorIndexShift())) > i) {
				GIV(classTableIndex) = ((sqInt)((usqInt)(((((i - 1) < 1) ? 1 : (i - 1)))) << (classTableMajorIndexShift())));
			}
			GIV(numClassTablePages) = i;
			assert((classOrNilAtIndex(GIV(classTableIndex))) == GIV(nilObj));
			goto l1;
		}
		else {
			if ((((usqInt)(GIV(classTableIndex))) >> (classTableMajorIndexShift())) > i) {
				j = 0;
				while (j < (1U << (classTableMajorIndexShift()))) {
					if ((longAt((void *)((page + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))))) == GIV(nilObj)) {
						GIV(classTableIndex) = ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
						j = 1U << (classTableMajorIndexShift());
					}
					j += 1;
				}
			}
		}
	}

	/* no unused slots; set it to the start of the second page. */
	if ((((usqInt)(GIV(classTableIndex))) >> (classTableMajorIndexShift())) >= GIV(numClassTablePages)) {
		GIV(classTableIndex) = 1U << (classTableMajorIndexShift());
	}
	assert((classOrNilAtIndex(GIV(classTableIndex))) == GIV(nilObj));
	/* end setHiddenRootsObj: */
l1:
	GIV(markStack) = swizzleObjStackAt(MarkStackRootIndex);
	GIV(weaklingStack) = swizzleObjStackAt(WeaklingStackRootIndex);
	GIV(mournQueue) = swizzleObjStackAt(MournQueueRootIndex);
	assert(validObjStacks());
	assert(isEmptyObjStack(GIV(markStack)));
	assert(isEmptyObjStack(GIV(weaklingStack)));

	/* begin initializeFreeSpacePostLoad: */
	assert((numSlotsOf(freeListObj)) == (numFreeLists()));
	assert((formatOf(freeListObj)) == (wordIndexableFormat()));
	GIV(freeLists) = firstIndexableField(freeListObj);
	GIV(freeListsMask) = 0;
	for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
		if (GIV(freeLists)[i]) {
			GIV(freeListsMask) = GIV(freeListsMask) | (1ULL << i);
			GIV(freeLists)[i] = (swizzleObj(GIV(freeLists)[i]));
		}
	}
	collapseSegmentsPostSwizzle();

	/* begin updateFreeLists */
	min = 3;
	for (i = min; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
		updateListStartingAt(GIV(freeLists)[i]);
	}

	/* Large chunks */

	/* begin freeTreeNodesDo: */
	treeNode = GIV(freeLists)[0];
	if (!treeNode) {
		goto l2;
	}
	cameFrom = -1;
	do {
		assert((bytesInBody(treeNode)) >= ((numFreeLists()) * (allocationUnit())));
		smallChild = longAt((void *)((treeNode + BaseHeaderSize) + (3U << (shiftForWord()))));
		largeChild = longAt((void *)((treeNode + BaseHeaderSize) + (4U << (shiftForWord()))));
		assert((smallChild == 0)
		 || (treeNode == (fetchPointerofFreeChunk(freeChunkParentIndex(), smallChild))));
		assert((largeChild == 0)
		 || (treeNode == (fetchPointerofFreeChunk(freeChunkParentIndex(), largeChild))));

		/* apply if the node has no children, or it has no large children and we're
		   returning from the small child, or we're returning from the large child. */
		if (((smallChild == 0)
		 && (largeChild == 0))
		 || ((largeChild
				? cameFrom == largeChild
				: cameFrom == smallChild))) {
			updateListStartingAt(treeNode);

			/* and since we've applied we must move on up */
			cameFrom = treeNode;
			treeNode = longAt((void *)((treeNode + BaseHeaderSize) + (2U << (shiftForWord()))));
		}
		else {
			if ((smallChild != 0)
			 && (cameFrom != smallChild)) {
				treeNode = smallChild;
			}
			else {
				assert(largeChild != 0);
				treeNode = largeChild;
			}
			cameFrom = -1;
		}
	} while(treeNode != 0);
	/* end freeTreeNodesDo: */
l2:

	/* begin computeFreeSpacePostSwizzle */
	GIV(totalFreeOldSpace) = totalFreeListBytes();
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	startOfFreeOldSpace = GIV(freeOldSpaceStart);

	/* begin initializeOldSpaceFirstFree: */
	limit = GIV(endOfMemory) - (2 * BaseHeaderSize);
	if (limit > startOfFreeOldSpace) {
		GIV(totalFreeOldSpace) += limit - startOfFreeOldSpace;
		freeOldStart = startOfFreeOldSpace;
		while ((limit - freeOldStart) >= (0x100000000LL)) {
			freeChunk = freeChunkWithBytesat(0x100000000LL, freeOldStart);
			freeOldStart += 0x100000000LL;
			assert(freeOldStart == (addressAfter(freeChunk)));
		}
		if (freeOldStart < limit) {
			freeChunk = freeChunkWithBytesat(limit - freeOldStart, freeOldStart);
			assert((addressAfter(freeChunk)) == limit);
		}
	}
	GIV(endOfMemory) -= 2 * BaseHeaderSize;
	GIV(freeOldSpaceStart) = GIV(endOfMemory);

	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCCheckFreeSpace)) == (GCCheckFreeSpace | GCCheckFreeSpace))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}
	initializeNewSpaceVariables();

	/* begin initializeRememberedSet */
	obj = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord()))))));
	if (obj == GIV(nilObj)) {
		/* begin allocatePinnedSlots: */
		obj = allocateSlotsForPinningInOldSpacebytesformatclassIndex(0x400, (1024U << (shiftForWord())) + (BaseHeaderSize + BaseHeaderSize), sixtyFourBitIndexableFormat(), sixtyFourBitLongsClassIndexPun());
		if (obj) {
			assert(isPinned(obj));

			/* begin fillObj:numSlots:with: */
			assert(oopisLessThan(((obj + BaseHeaderSize) + (0x400 * BytesPerOop)) - 1, addressAfter(obj)));
			toDoLimit = ((usqInt)(((obj + BaseHeaderSize) + (0x400 * BytesPerOop)) - 1));
			for (p = (((usqInt)(obj + BaseHeaderSize))); p <= toDoLimit; p += 8 /* allocationUnit */) {
				longAtput((void *)(p),0);
			}
		}

		/* begin rememberedSetObj: */
		assert(isOldObject(obj));
		assert((isNonImmediate(GIV(hiddenRootsObj)))
		 && (!(isForwarded(GIV(hiddenRootsObj)))));
		assert(validStorePointerUncheckedArgs(RememberedSetRootIndex, GIV(hiddenRootsObj), obj));
		longAtput((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord()))))),obj);
	}
	else {
		/* begin setFormatOf:to: */
		assert((((sixtyFourBitIndexableFormat()) >= 0) && ((sixtyFourBitIndexableFormat()) <= (formatMask()))));
		byteAtput((void *)(obj + (formatFieldByteOffset())),((byteAt((void *)(obj + (formatFieldByteOffset())))) & (0xFF - (formatMask()))) + (sixtyFourBitIndexableFormat()));
	}

	/* The Spur32to64BitBootstrap failed to set the type of rememberedSetObj to 64-bit indexability.
	   This is unimportant except for simulation; rememberedSet is declared as sqInt *, but to have
	   firstIndexableField: below answer a suitable type the format must be wordIndexableFormat. */
	assert((formatOf(obj)) == (wordIndexableFormat()));
	assert(isPinned(obj));
	GIV(rememberedSet) = firstIndexableField(obj);
	GIV(rememberedSetSize) = 0;

	/* begin numSlotsOf: */
	assert((classIndexOf(obj)) > (isForwardedObjectClassIndexPun()));
	GIV(rememberedSetLimit) = (((numSlots = byteAt((void *)(obj + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(obj - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);

	/* begin setRememberedSetRedZone */
	fudge = ((((GIV(eden).limit)) - ((GIV(eden).start))) / BytesPerWord) / 0x400;
	GIV(rememberedSetRedZone) = ((((GIV(rememberedSetLimit) * 3) / 4) < fudge) ? fudge : ((GIV(rememberedSetLimit) * 3) / 4));
	checkSegments();

	/* begin biasForGC */
	GIV(biasForGC) = 1;

	/* These defaults should depend on machine size; e.g. too small on a powerful laptop, too big on a Pi. */

	/* headroom when growing */
	GIV(growHeadroom) = 0x1000000;

	/* free space before shrinking */
	GIV(shrinkThreshold) = 0x2000000;
	GIV(heapSizeAtPreviousGC) = ((assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges) - (GIV(numSegments) * (2 * BaseHeaderSize)))) - GIV(totalFreeOldSpace);

	/* begin resetAllocationAccountingAfterGC */
	GIV(oldSpaceUsePriorToScavenge) = ((assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges) - (GIV(numSegments) * (2 * BaseHeaderSize)))) - GIV(totalFreeOldSpace);

	/* By default GC after scavenge if heap has grown by a third since the last GC */
	GIV(heapGrowthToSizeGCRatio) = 0.333333;
}
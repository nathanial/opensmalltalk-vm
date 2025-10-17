/* Extracted from interp.c:40280 (function markObjects). */

/*	for profiling */
/*	Mark all accessible objects.
	objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged is true if all
	objects are unmarked and/or if unmarked classes shoud be removed from the
	class table. */
/*	If the incremental collector is running mark bits may be set; stop it and
	clear them if necessary.
 */

	/* SpurMemoryManager>>#markObjects: */

static NoDbgRegParms NeverInline void
markObjects(sqInt objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classIndex;
    sqInt classOrNil;
    sqInt classTablePage;
    sqInt format;
    sqInt i;
    sqInt j;
    sqInt largestFree;
    sqInt objOop;
    sqInt oop;
    sqInt result;
    sqInt sizeOfAvailableNewSpace;
    sqInt sizeOfFutureSpace;
    sqInt sizeOfUnusedEden;
    StackPage *thePage;
    sqInt toDoLimit;

	runLeakCheckerFor(GCModeFull);

	/* begin shutDownGlobalIncrementalGC: */
	if (objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged) {
		assert(allObjectsUnmarked());
	}

	/* begin initializeUnscannedEphemerons */
	largestFree = findLargestFreeChunk();
	sizeOfUnusedEden = (((GIV(eden)).limit)) - GIV(freeStart);
	sizeOfFutureSpace = (((GIV(futureSpace)).limit)) - (((GIV(futureSpace)).start));
	sizeOfAvailableNewSpace = ((sizeOfUnusedEden < sizeOfFutureSpace) ? sizeOfFutureSpace : sizeOfUnusedEden);
	if ((largestFree)
	 && ((numSlotsOfAny(largestFree)) > (sizeOfAvailableNewSpace / BytesPerWord))) {
		(GIV(unscannedEphemerons).start = (largestFree + BaseHeaderSize) + (5 /* (freeChunkLargerIndex + 1) */ * BytesPerWord));
		(GIV(unscannedEphemerons).limit = addressAfter(largestFree));
	}
	else {
		if (sizeOfUnusedEden > sizeOfFutureSpace) {
			(GIV(unscannedEphemerons).start = GIV(freeStart));
			(GIV(unscannedEphemerons).limit = ((GIV(eden)).limit));
		}
		else {
			(GIV(unscannedEphemerons).start = ((GIV(futureSpace)).start));
			(GIV(unscannedEphemerons).limit = ((GIV(futureSpace)).limit));
		}
	}
	(GIV(unscannedEphemerons).top = ((GIV(unscannedEphemerons).start)) - BytesPerOop);

	/* begin initializeMarkStack */
	ensureRoomOnObjStackAt(MarkStackRootIndex);
	ensureRoomOnObjStackAt(WeaklingStackRootIndex);
	GIV(marking) = 1;

	/* begin markAccessibleObjectsAndFireEphemerons */
	assert(GIV(marking));
	assert(validClassTableRootPages());
	assert(allBridgesMarked());

	/* for debugging markAndTrace: set (MarkStackRecord := OrderedCollection new)
	   This must come first to enable stack page reclamation.  It clears
	   the trace flags on stack pages and so must precede any marking.
	   Otherwise it will clear the trace flags of reached pages. */

	/* begin initStackPageGC */
	if (GIV(stackPage)) {
		/* begin externalWriteBackHeadFramePointers */
		assert((GIV(framePointer) - GIV(stackPointer)) < (LargeContextSlots * BytesPerOop));
		assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
		assert(!((isFree(GIV(stackPage)))));

		/* begin setHeadFP:andSP:inPage: */
		assert(GIV(stackPointer) < GIV(framePointer));
		assert((GIV(stackPointer) < ((GIV(stackPage)->baseAddress)))
		 && (GIV(stackPointer) > (((GIV(stackPage)->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
		assert((GIV(framePointer) < ((GIV(stackPage)->baseAddress)))
		 && (GIV(framePointer) > (((GIV(stackPage)->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
		(GIV(stackPage)->headFP = GIV(framePointer));
		(GIV(stackPage)->headSP = GIV(stackPointer));
		assert(pageListIsWellFormed());
	}
	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		thePage = stackPageAtpages(i, GIV(pages));
		(thePage->trace = StackPageUnreached);
	}

	/* begin markAndTraceHiddenRoots */
	markAndTraceObjStackandContents(GIV(markStack), 0);
	markAndTraceObjStackandContents(GIV(weaklingStack), 0);
	markAndTraceObjStackandContents(GIV(mournQueue), 1);
	objOop = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(RememberedSetRootIndex) << (shiftForWord()))))));

	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(objOop)));
	byteAtput((void *)(objOop + (markBitsByteOffset())),(byteAt((void *)(objOop + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
	/* begin freeListsObj */
	assert((firstIndexableField(oldSpaceObjectAfter(GIV(trueObj)))) == GIV(freeLists));
	objOop = oldSpaceObjectAfter(GIV(trueObj));

	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(objOop)));
	byteAtput((void *)(objOop + (markBitsByteOffset())),(byteAt((void *)(objOop + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));

	/* begin isWeakNonImm: */
	format = (byteAt((void *)(GIV(classTableFirstPage) + (formatFieldByteOffset())))) & (formatMask());
	if (format == (weakArrayFormat())) {
		markAndTrace(GIV(hiddenRootsObj));
		goto l1;
	}

	/* begin setIsMarkedOf:to: */
	assert(!(isFreeObject(GIV(hiddenRootsObj))));
	byteAtput((void *)(GIV(hiddenRootsObj) + (markBitsByteOffset())),(byteAt((void *)(GIV(hiddenRootsObj) + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
	markAndTrace(GIV(classTableFirstPage));
	for (i = 1; i < GIV(numClassTablePages); i += 1) {
		objOop = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));

		/* begin setIsMarkedOf:to: */
		assert(!(isFreeObject(objOop)));
		byteAtput((void *)(objOop + (markBitsByteOffset())),(byteAt((void *)(objOop + (markBitsByteOffset())))) | (1U << (markedBitByteShift())));
	}
	/* end markAndTraceHiddenRoots */
l1:

	/* begin markAndTraceExtraRoots */
	assert(GIV(remapBufferCount) == 0);

	/* 1 to: remapBufferCount do:
	   [:i|
	   oop := remapBuffer at: i.
	   ((self isImmediate: oop) or: [self isFreeObject: oop]) ifFalse:
	   [self markAndTrace: oop]]. */
	for (i = 1; i <= GIV(extraRootCount); i += 1) {
		oop = (GIV(extraRoots)[i])[0];
		if (!((((oop & (tagMask())) != 0))
			 || (((longAt((void *)(oop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())))) {
			markAndTrace(oop);
		}
	}
	assert(validClassTableRootPages());

	/* begin markAndTraceInterpreterOops: */
	markAndTraceStackPages(1);
	markAndTraceTraceLog();
	markAndTracePrimTraceLog();
	markAndTrace(GIV(specialObjectsOop));
	if (!(((GIV(newMethod) & (tagMask())) != 0))) {
		markAndTrace(GIV(newMethod));
	}

	/* begin traceProfileState */
	/* begin followForwardingPointersInProfileState */
	/* begin profileStateDoUpdating: */
	if (GIV(profileProcess)) {
		if ((result = ((!((longAt((void *)(GIV(profileProcess)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
					? followForwarded(GIV(profileProcess))
					: 0))) {
			GIV(profileProcess) = result;
		}
	}
	if (GIV(profileMethod)) {
		if ((result = ((!((longAt((void *)(GIV(profileMethod)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
					? followForwarded(GIV(profileMethod))
					: 0))) {
			GIV(profileMethod) = result;
		}
	}
	if (GIV(profileSemaphore)) {
		if ((result = ((!((longAt((void *)(GIV(profileSemaphore)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
					? followForwarded(GIV(profileSemaphore))
					: 0))) {
			GIV(profileSemaphore) = result;
		}
	}

	/* begin profileStateDo: */
	if (GIV(profileProcess)) {
		markAndTrace(GIV(profileProcess));
	}
	if (GIV(profileMethod)) {
		markAndTrace(GIV(profileMethod));
	}
	if (GIV(profileSemaphore)) {
		markAndTrace(GIV(profileSemaphore));
	}
#  if LRPCheck
	sqLowLevelMFence();
	if ((GIV(longRunningPrimitiveCheckMethod) != null)
	 && (GIV(longRunningPrimitiveCheckSequenceNumber) != GIV(statCheckForEvents))) {
		if ((!((longAt((void *)(GIV(longRunningPrimitiveCheckMethod)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			GIV(longRunningPrimitiveCheckMethod) = followForwarded(GIV(longRunningPrimitiveCheckMethod));
		}
		markAndTrace(GIV(longRunningPrimitiveCheckMethod));
	}
	if (GIV(longRunningPrimitiveCheckSemaphore)) {
		if ((!((longAt((void *)(GIV(longRunningPrimitiveCheckSemaphore)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			GIV(longRunningPrimitiveCheckSemaphore) = followForwarded(GIV(longRunningPrimitiveCheckSemaphore));
		}
		markAndTrace(GIV(longRunningPrimitiveCheckSemaphore));
	}
#  endif // LRPCheck

	if (GIV(tempOop)) {
		markAndTrace(GIV(tempOop));
	}
	if (GIV(tempOop2)) {
		markAndTrace(GIV(tempOop2));
	}

	/* V3 memory manager support */
	for (i = 1; i <= GIV(remapBufferCount); i += 1) {
		oop = GIV(remapBuffer)[i];
		if (!(((oop & (tagMask())) != 0))) {
			markAndTrace(oop);
		}
	}
	assert(validObjStacks());
	markWeaklingsAndMarkAndFireEphemerons();
	assert(validObjStacks());

	/* begin expungeDuplicateAndUnmarkedClasses: */
	for (i = 1; i < GIV(numClassTablePages); i += 1) {
		classTablePage = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
		for (j = 0; j <= toDoLimit; j += 1) {
			classOrNil = longAt((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))));
			classIndex = ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
			assert((classOrNil == GIV(nilObj))
			 || (addressCouldBeClassObj(classOrNil)));

			/* only remove a class if it is at a duplicate entry or it is unmarked and we're expunging unmarked classes. */
			if (classOrNil == GIV(nilObj)) {
				if (classIndex < GIV(classTableIndex)) {
					GIV(classTableIndex) = classIndex;
				}
			}
			else {
				if ((objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged
				 && (!(((byteAt((void *)(classOrNil + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)))
				 || (((long32At((void *)(classOrNil + 4))) & (identityHashHalfWordMask())) != classIndex)) {
					/* begin storePointerUnchecked:ofObject:withValue: */
					assert((isNonImmediate(classTablePage))
					 && (!(isForwarded(classTablePage))));
					assert(validStorePointerUncheckedArgs(j, classTablePage, GIV(nilObj)));
					longAtput((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))),GIV(nilObj));

					/* but if it is marked, it should still be in the table at its correct index. */
					assert((objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged
					 && (!(isMarked(classOrNil))))
					 || ((classAtIndex(rawHashBitsOf(classOrNil))) == classOrNil));

					/* If the removed class is before the classTableIndex, set the
					   classTableIndex to point to the empty slot so as to reuse it asap. */
					if (classIndex < GIV(classTableIndex)) {
						GIV(classTableIndex) = classIndex;
					}
				}
			}
		}
	}

	/* Avoid expunging the puns by not scanning the 0th page.
	   classTableIndex must never index the first page, which is reserved for classes known to the VM. */
	assert(GIV(classTableIndex) >= (1U << (classTableMajorIndexShift())));
	nilUnmarkedWeaklingSlots();
	GIV(marking) = 0;
}
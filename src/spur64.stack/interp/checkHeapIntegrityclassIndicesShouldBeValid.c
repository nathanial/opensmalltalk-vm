/* Extracted from interp.c:32289 (function checkHeapIntegrityclassIndicesShouldBeValid). */

/*	Perform an integrity/leak check using the heapMap. Assume
	clearLeakMapAndMapAccessibleObjects has set a bit at each (non-free)
	object's header. Scan all objects in the heap checking that every
	pointer points to a header. Scan the rememberedSet, remapBuffer and
	extraRootTable checking
	that every entry is a pointer to a header. Check that the number of roots
	is correct and that all
	rememberedSet entries have their isRemembered: flag set. Answer if all
	checks pass. */

	/* SpurMemoryManager>>#checkHeapIntegrity:classIndicesShouldBeValid: */

static NoDbgRegParms sqInt
checkHeapIntegrityclassIndicesShouldBeValid(sqInt excludeUnmarkedObjs, sqInt classIndicesShouldBeValid)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt chunkBytes;
    sqInt classIndex;
    sqInt classIndexSqInt;
    sqInt classOop;
    sqInt classTablePage;
    sqInt containsYoung;
    sqInt fi;
    sqInt fieldIndex;
    sqInt fieldOop;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt freeListOop;
    sqInt i;
    sqInt iSqInt;
    sqInt numOnThisPage;
    sqInt numRememberedObjectsInHeap;
    usqInt numSlots;
    sqInt obj;
    sqInt objOop;
    sqInt objStackPage;
    sqInt ok;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt ri;
    usqInt start;
    sqInt toDoLimit;
    sqInt toDoLimit1;

	/* Almost all of the time spent here used to go into the asserts in fetchPointer:ofObject: in the
	   simulator class overrides. Since we know here the indices used are valid we temporarily
	   remove them to claw back that performance. */
	ok = 1;
	numRememberedObjectsInHeap = 0;
	for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
		if ((freeListOop = GIV(freeLists)[i])) {
			if (oopisGreaterThanOrEqualToandLessThan(freeListOop, GIV(oldSpaceStart), GIV(endOfMemory))) {
				if (heapMapAtWord(pointerForOop(freeListOop))) {
					fprintf(GIV(transcript),
							"leak in free list %d to non-free %p\n",
							((int) i),
							((void *)freeListOop));
					ok = 0;
				}
			}
			else {
				fprintf(GIV(transcript),
						"leak in free list %d to out-of-bounds %p\n",
						((int) i),
						((void *)freeListOop));
				ok = 0;
			}
		}
	}

	/* Excuse the duplication but performance is at a premium and we avoid
	   some tests by splitting the newSpace and oldSpace enumerations. */

	/* begin allNewSpaceEntitiesDo: */
	prevPrevObj = (prevObj = null);

	/* After a scavenge eden is empty, futureSpace is empty, and all newSpace objects are
	   in pastSpace.  Objects are allocated in eden.  So enumerate only pastSpace and eden. */
	assert((((GIV(pastSpace)).start)) < (((GIV(eden)).start)));
	start = /* startAddressForBridgedHeapEnumeration */
			(GIV(pastSpaceStart) > (((GIV(pastSpace)).start))
				? ((GIV(pastSpace)).start)
				: (GIV(freeStart) > (((GIV(eden)).start))
						? ((GIV(eden)).start)
						: GIV(oldSpaceStart)));
	if (start > GIV(freeStart)) {
		goto l6;
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
	numSlots = byteAt((void *)(start + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? start + BaseHeaderSize
				: start);
	while (oopisLessThan(objOop, GIV(freeStart))) {
		if (((longAt((void *)(objOop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())) {
			fprintf(GIV(transcript),
					"young object %p is free\n",
					((void *)objOop));
			ok = 0;
		}
		else {
			if (!((!(((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))
				 && (excludeUnmarkedObjs))) {
				if ((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))) {
					fprintf(GIV(transcript),
							"young object %p is remembered\n",
							((void *)objOop));
					ok = 0;
				}
			}
			if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				fieldOop = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
				if (/* isInHeapBounds: */
					(oopisGreaterThanOrEqualTo(fieldOop, GIV(newSpaceStart)))
				 && (oopisLessThan(fieldOop, GIV(endOfMemory)))) {
					if (!(heapMapAtWord(pointerForOop(fieldOop)))) {
						fprintf(GIV(transcript),
								"object leak in forwarder %p to unmapped %p\n",
								((void *)objOop),
								((void *)fieldOop));
						ok = 0;
					}
				}
				else {
					fprintf(GIV(transcript),
							"object leak in forwarder %p to out-of-bounds %p\n",
							((void *)objOop),
							((void *)fieldOop));
					ok = 0;
				}
			}
			else {
				classIndexSqInt = (classIndex = (longAt((void *)(objOop))) & (classIndexMask()));

				/* begin classOrNilAtIndex: */
				assert((classIndexSqInt <= (tagMask()))
				 || (classIndexSqInt >= (arrayClassIndexPun())));
				fieldIndex = ((usqInt)(classIndexSqInt)) >> (classTableMajorIndexShift());

				/* begin fetchPointer:ofObject: */
				classTablePage = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
				if (classTablePage == GIV(nilObj)) {
					classOop = GIV(nilObj);
					goto l1;
				}
				fieldIndex = classIndexSqInt & ((1U << (classTableMajorIndexShift())) - 1);
				classOop = longAt((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
				/* end classOrNilAtIndex: */
l1:
				if (classIndicesShouldBeValid
				 && ((classOop == GIV(nilObj))
				 && (!(((longAt((void *)(objOop))) & (classIndexMask())) <= (lastClassIndexPun()))))) {
					fprintf(GIV(transcript),
							"object leak in %p invalid class index %p -> nilObj\n",
							((void *)objOop),
							((void *)classIndex));
					ok = 0;
				}
				toDoLimit = (numPointerSlotsOf(objOop)) - 1;
				for (fi = 0; fi <= toDoLimit; fi += 1) {
					fieldOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fi) << (shiftForWord()))))));
					if ((!(fieldOop & (tagMask())))) {
						if (/* isInHeapBounds: */
							(oopisGreaterThanOrEqualTo(fieldOop, GIV(newSpaceStart)))
						 && (oopisLessThan(fieldOop, GIV(endOfMemory)))) {
							if (!(heapMapAtWord(pointerForOop(fieldOop)))) {
								fprintf(GIV(transcript),
										"object leak in %p @ %d = %p\n",
										((void *)objOop),
										((int) fi),
										((void *)fieldOop));
								ok = 0;
							}
						}
						else {
							fprintf(GIV(transcript),
									"object leak in %p to out-of-bounds %p\n",
									((void *)objOop),
									((void *)fieldOop));
							ok = 0;
						}
					}
				}
			}
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(freeStart))) {
			objOop = GIV(freeStart);
			goto l5;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOop, GIV(oldSpaceStart)))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l5:;
	}
	/* end allNewSpaceEntitiesDo: */
l6:

	/* begin allOldSpaceEntitiesDo: */
	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(GIV(nilObj)));
	prevPrevObj = (prevObj = null);
	objOop = GIV(nilObj);
	while (1) {
		assert((objOop % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOop, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objOop))) != 0);
		if (((longAt((void *)(objOop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())) {
			if (heapMapAtWord(pointerForOop(objOop))) {
				fprintf(GIV(transcript),
						"leak in free chunk %p is mapped?! \n",
						((void *)objOop));
				ok = 0;
			}
			fieldOop = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
			if ((fieldOop != 0)
			 && ((heapMapAtWord(pointerForOop(fieldOop))) != 0)) {
				fprintf(GIV(transcript),
						"leak in free chunk %p @ 0 = %p is mapped\n",
						((void *)objOop),
						((void *)fieldOop));
				ok = 0;
			}
			chunkBytes = bytesInBody(objOop);

			/* begin isLilliputianSize: */
			assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
			if (!(chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */))) {
				fieldOop = longAt((void *)((objOop + BaseHeaderSize) + (1U << (shiftForWord()))));
				if ((fieldOop != 0)
				 && ((heapMapAtWord(pointerForOop(fieldOop))) != 0)) {
					fprintf(GIV(transcript),
							"leak in free chunk %p @ 1 = %p is mapped\n",
							((void *)objOop),
							((void *)fieldOop));
					ok = 0;
				}
			}
			if ((bytesInBody(objOop)) >= 0x200 /* (numFreeLists * #allocationUnit) */) {
				for (fi = 2 /* freeChunkParentIndex */; fi <= 4 /* freeChunkLargerIndex */; fi += 1) {
					fieldOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fi) << (shiftForWord()))))));
					if ((fieldOop != 0)
					 && ((heapMapAtWord(pointerForOop(fieldOop))) != 0)) {
						fprintf(GIV(transcript),
								"leak in free chunk %p @ %d = %p is mapped\n",
								((void *)objOop),
								((int) fi),
								((void *)fieldOop));
						ok = 0;
					}
				}
			}
		}
		else {
			if (!(excludeUnmarkedObjs
				 && (!(((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)))) {
				containsYoung = 0;
				if ((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))) {
					numRememberedObjectsInHeap += 1;

					/* begin isInRememberedSet: */
					for (iSqInt = 0; iSqInt < GIV(rememberedSetSize); iSqInt += 1) {
						if ((GIV(rememberedSet)[iSqInt]) == objOop) {
							goto l2;
						}
					}
					fprintf(GIV(transcript),
							"remembered object %p is not in remembered table\n",
							((void *)objOop));
					ok = 0;
l2:;
				}
				if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					fieldOop = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
					if (/* isInHeapBounds: */
						(oopisGreaterThanOrEqualTo(fieldOop, GIV(newSpaceStart)))
					 && (oopisLessThan(fieldOop, GIV(endOfMemory)))) {
						if (!(heapMapAtWord(pointerForOop(fieldOop)))) {
							fprintf(GIV(transcript),
									"object leak in forwarder %p to unmapped %p\n",
									((void *)objOop),
									((void *)fieldOop));
							ok = 0;
						}
					}
					else {
						fprintf(GIV(transcript),
								"object leak in forwarder %p to out-of-bounds %p\n",
								((void *)objOop),
								((void *)fieldOop));
						ok = 0;
					}
					if (/* isReallyYoung: */
						((!(fieldOop & (tagMask()))))
					 && ((/* begin isReallyYoungObject: *//* begin isYoungObject: */
						assert(isNonImmediate(fieldOop)),
					oopisLessThan(fieldOop, GIV(oldSpaceStart))))) {
						containsYoung = 1;
					}
				}
				else {
					classIndexSqInt = (classIndex = (longAt((void *)(objOop))) & (classIndexMask()));

					/* begin classOrNilAtIndex: */
					assert((classIndexSqInt <= (tagMask()))
					 || (classIndexSqInt >= (arrayClassIndexPun())));
					fieldIndex = ((usqInt)(classIndexSqInt)) >> (classTableMajorIndexShift());

					/* begin fetchPointer:ofObject: */
					classTablePage = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
					if (classTablePage == GIV(nilObj)) {
						classOop = GIV(nilObj);
						goto l3;
					}
					fieldIndex = classIndexSqInt & ((1U << (classTableMajorIndexShift())) - 1);
					classOop = longAt((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
					/* end classOrNilAtIndex: */
l3:
					if (classIndicesShouldBeValid
					 && ((classOop == GIV(nilObj))
					 && (classIndex > (lastClassIndexPun())))) {
						fprintf(GIV(transcript),
								"object leak in %p invalid class index %p -> nilObj\n",
								((void *)objOop),
								((void *)classIndex));
						ok = 0;
					}
					toDoLimit1 = (numPointerSlotsOf(objOop)) - 1;
					for (fi = 0; fi <= toDoLimit1; fi += 1) {
						fieldOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fi) << (shiftForWord()))))));
						if ((!(fieldOop & (tagMask())))) {
							if (/* isInHeapBounds: */
								(oopisGreaterThanOrEqualTo(fieldOop, GIV(newSpaceStart)))
							 && (oopisLessThan(fieldOop, GIV(endOfMemory)))) {
								if (!(heapMapAtWord(pointerForOop(fieldOop)))) {
									fprintf(GIV(transcript),
											"object leak in %p @ %d = %p\n",
											((void *)objOop),
											((int) fi),
											((void *)fieldOop));
									ok = 0;
								}
							}
							else {
								fprintf(GIV(transcript),
										"object leak in %p to out-of-bounds %p\n",
										((void *)objOop),
										((void *)fieldOop));
								ok = 0;
							}

							/* don't be misled by CogMethods; they appear to be young, but they're not */
							if (/* isReallyYoung: */
								((!(fieldOop & (tagMask()))))
							 && ((/* begin isReallyYoungObject: *//* begin isYoungObject: */
								assert(isNonImmediate(fieldOop)),
							oopisLessThan(fieldOop, GIV(oldSpaceStart))))) {
								containsYoung = 1;
							}
						}
					}
				}
				if (containsYoung) {
					if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						fprintf(GIV(transcript),
								"unremembered object %p contains young oop(s)\n",
								((void *)objOop));
						ok = 0;
					}
				}
			}
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			objOop = GIV(endOfMemory);
			goto l7;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l7:;
	}
	if (numRememberedObjectsInHeap != GIV(rememberedSetSize)) {
		fprintf(GIV(transcript),
				"root count mismatch. #heap roots %d; #roots %d\n",
				((int) numRememberedObjectsInHeap),
				((int) GIV(rememberedSetSize)));

		/* But the system copes with overflow... */
	}

	/* begin rememberedSetWithIndexDo: */
	for (iSqInt = 0; iSqInt < GIV(rememberedSetSize); iSqInt += 1) {
		obj = GIV(rememberedSet)[iSqInt];
		if (obj & (BytesPerWord - 1)) {
			fprintf(GIV(transcript),
					"misaligned oop in remembered set @ %d = %p\n",
					((int) iSqInt),
					((void *)obj));
			ok = 0;
		}
		else {
			if (heapMapAtWord(pointerForOop(obj))) {
				if (/* isYoung: */
					((!(obj & (tagMask()))))
				 && (oopisLessThan(obj, GIV(oldSpaceStart)))) {
					fprintf(GIV(transcript),
							"non-root in remembered set @ %d = %p\n",
							((int) iSqInt),
							((void *)obj));
					ok = 0;
				}
			}
			else {
				fprintf(GIV(transcript),
						"object leak in remembered set @ %d = %p\n",
						((int) iSqInt),
						((void *)obj));
				ok = 0;
			}
		}
	}

	/* begin objStack:do: */
	if (GIV(mournQueue) == GIV(nilObj)) {
		goto l4;
	}
	eassert(isValidObjStack(GIV(mournQueue)));
	objStackPage = GIV(mournQueue);
	while (objStackPage != 0) {
		numOnThisPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
		for (iSqInt = ((numOnThisPage + ObjStackFixedSlots) - 1); iSqInt >= ObjStackFixedSlots; iSqInt += -1) {
			obj = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))));
			if (obj & (BytesPerWord - 1)) {
				fprintf(GIV(transcript),
						"misaligned oop in mournQueue @ %d in %p = %p\n",
						((int) iSqInt),
						((void *)objStackPage),
						((void *)obj));
				ok = 0;
			}
			else {
				if (!(excludeUnmarkedObjs
					 && (!(((byteAt((void *)(obj + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)))) {
					if (!(heapMapAtWord(pointerForOop(obj)))) {
						fprintf(GIV(transcript),
								"object leak in mournQueue @ %d in %p = %p\n",
								((int) iSqInt),
								((void *)objStackPage),
								((void *)obj));
						ok = 0;
					}
				}
			}
		}
		objStackPage = longAt((void *)((objStackPage + BaseHeaderSize) + ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
	}
	/* end objStack:do: */
l4:
	for (ri = 1; ri <= GIV(remapBufferCount); ri += 1) {
		obj = GIV(remapBuffer)[ri];
		if (obj & (BytesPerWord - 1)) {
			fprintf(GIV(transcript),
					"misaligned remapRoot @ %d = %p\n",
					((int) ri),
					((void *)obj));
			ok = 0;
		}
		else {
			if (!(heapMapAtWord(pointerForOop(obj)))) {
				fprintf(GIV(transcript),
						"object leak in remapRoots @ %d = %p\n",
						((int) ri),
						((void *)obj));
				ok = 0;
			}
		}
	}
	for (ri = 1; ri <= GIV(extraRootCount); ri += 1) {
		obj = (GIV(extraRoots)[ri])[0];
		if (obj & (BytesPerWord - 1)) {
			fprintf(GIV(transcript),
					"misaligned extraRoot @ %d => %p\n",
					((int) ri),
					((void *)obj));
			ok = 0;
		}
		else {
			if (!(heapMapAtWord(pointerForOop(obj)))) {
				fprintf(GIV(transcript),
						"object leak in extraRoots @ %d => %p\n",
						((int) ri),
						((void *)obj));
				ok = 0;
			}
		}
	}
	return ok;
}
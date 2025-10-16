/* Extracted from interp.c:32052 (function checkHeapFreeSpaceIntegrity). */

static sqInt
checkHeapFreeSpaceIntegrity(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt chunkBytes;
    sqInt fi;
    sqInt fieldOop;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt i;
    usqInt numSlots;
    sqInt objOop;
    sqInt ok;
    sqInt prevObj;
    sqInt prevPrevObj;
    usqInt start;
    sqInt toDoLimit;
    sqInt toDoLimit1;
    usqInt total;

	ok = 1;
	total = 0;
	for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
		if (GIV(freeLists)[i]) {
			if (!(heapMapAtWord(pointerForOop(GIV(freeLists)[i])))) {
				fprintf(GIV(transcript),
						"leak in free list %d to non-free %p\n",
						((int) i),
						((void *)(GIV(freeLists)[i])));
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
		goto l2;
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
			if (objOop != GIV(freeSpaceCheckOopToIgnore)) {
				toDoLimit = (numPointerSlotsOf(objOop)) - 1;
				for (fi = 0; fi <= toDoLimit; fi += 1) {
					fieldOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fi) << (shiftForWord()))))));
					if ((!(fieldOop & (tagMask())))) {
						if (heapMapAtWord(pointerForOop(fieldOop))) {
							fprintf(GIV(transcript),
									"object leak in %p @ %d = %p is free\n",
									((void *)objOop),
									((int) fi),
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
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOop, GIV(oldSpaceStart)))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l1:;
	}
	/* end allNewSpaceEntitiesDo: */
l2:

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
			if (!(heapMapAtWord(pointerForOop(objOop)))) {
				fprintf(GIV(transcript),
						"leak in free chunk %p is unmapped?! \n",
						((void *)objOop));
				ok = 0;
			}
			fieldOop = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
			if ((fieldOop != 0)
			 && ((heapMapAtWord(pointerForOop(fieldOop))) == 0)) {
				fprintf(GIV(transcript),
						"leak in free chunk %p @ 0 = %p is unmapped\n",
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
				 && ((heapMapAtWord(pointerForOop(fieldOop))) == 0)) {
					fprintf(GIV(transcript),
							"leak in free chunk %p @ 0 = %p is unmapped\n",
							((void *)objOop),
							((void *)fieldOop));
					ok = 0;
				}
			}
			if ((bytesInBody(objOop)) >= 0x200 /* (numFreeLists * #allocationUnit) */) {
				for (fi = 2 /* freeChunkParentIndex */; fi <= 4 /* freeChunkLargerIndex */; fi += 1) {
					fieldOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fi) << (shiftForWord()))))));
					if ((fieldOop != 0)
					 && ((heapMapAtWord(pointerForOop(fieldOop))) == 0)) {
						fprintf(GIV(transcript),
								"leak in free chunk %p @ %d = %p is unmapped\n",
								((void *)objOop),
								((int) fi),
								((void *)fieldOop));
						ok = 0;
					}
				}
			}
			total += bytesInBody(objOop);
		}
		else {
			if (objOop != GIV(freeSpaceCheckOopToIgnore)) {
				toDoLimit1 = (numPointerSlotsOf(objOop)) - 1;
				for (fi = 0; fi <= toDoLimit1; fi += 1) {
					if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
						assert(fi == 0);
						fieldOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fi) << (shiftForWord()))))));
					}
					else {
						fieldOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(fi) << (shiftForWord()))))));
					}

					/* We keep #fetchPointer:ofObject: API here for assertions */
					if ((!(fieldOop & (tagMask())))) {
						if (heapMapAtWord(pointerForOop(fieldOop))) {
							fprintf(GIV(transcript),
									"object leak in %p @ %d = %p is free\n",
									((void *)objOop),
									((int) fi),
									((void *)fieldOop));
							ok = 0;
						}
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
			goto l3;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l3:;
	}
	if (total != GIV(totalFreeOldSpace)) {
		fprintf(GIV(transcript),
				"incorrect totalFreeOldSpace; expected %d found %d\n",
				((int) GIV(totalFreeOldSpace)),
				((int) total));
		ok = 0;
	}
	return ok;
}
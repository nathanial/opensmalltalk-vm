/* Extracted from interp.c:47255 (function copyAndUnmarkMobileObjects). */

/*	Sweep the mobile portion of the heap, moving objects to their eventual
	locations, and clearing their marked bits.
	Remember to update the savedFirstFields of pointer objects, as these have
	been forwarded.
	Answer if the end of the heap was reached (savedFirstFieldsSpace has not
	overflowed). 
	The enumerations in planCompactSavingForwarders,
	updatePointersInMobileObjects and copyAndUnmarkMobileObjects
	match. We could implement them as a single enumeration method taking
	several block arguments, but arguably that
	would make understanding an already tricky algorithm more difficult.
	Instead we tolerate the duplication and encourage
	the reader to diff the three methods to see where they diverge (e.g. via
	Cmd-shift-C).  */

	/* SpurPlanningCompactor>>#copyAndUnmarkMobileObjects */

static NeverInline sqInt
copyAndUnmarkMobileObjects(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt availableSpace;
    usqInt bytes;
    sqInt destObj;
    sqInt finalObject;
    sqInt firstField;
    sqInt followingWord;
    usqInt followingWordAddress;
    usqInt limit;
    usqInt next;
    usqInt nextBytes;
    sqInt nextObj;
    usqInt numSlots;
    usqInt obj;
    usqInt objBytes;
    sqInt objOop;
    usqInt previousPin;
    sqInt prevObj;
    sqInt prevPrevObj;
    SpurSegmentInfo *seg;
    usqInt start;
    usqInt startOfPreviousPin;
    usqInt toFinger;
    usqInt top;

	availableSpace = 0;
	previousPin = 0;
	assert(!((isMarked(firstFreeObject))));
	toFinger = /* startOfObject: */
			((byteAt((void *)(firstFreeObject + (numSlotsFieldByteOffset())))) == (numSlotsMask())
				? firstFreeObject - BaseHeaderSize
				: firstFreeObject);
	top = (savedFirstFieldsSpace.start);
	startOfPreviousPin = 0;
	finalObject = (!(lastMobileObject)
			? nilObj
			: lastMobileObject);

	/* begin allOldSpaceEntitiesForCompactingFrom:to:do: */
	assert(isOldObject(firstFreeObject));
	assert(oopisLessThanOrEqualTo(finalObject, endOfMemory));
	prevPrevObj = (prevObj = null);
	objOop = firstFreeObject;
	limit = (oopisLessThan(finalObject, endOfMemory)
				? addressAfter(finalObject)
				: endOfMemory);
	while (1) {
		assert((objOop % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOop, limit))) break;
		assert((long64At((void *)(objOop))) != 0);

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
			nextObj = endOfMemory;
			goto l1;
		}
		followingWord = longAt((void *)(followingWordAddress));
		nextObj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l1:
		assert((previousPin == null
				? toFinger <= (startOfObject(objOop))
				: (isMarked(previousPin))
				 && (toFinger <= startOfPreviousPin)));
		assert(savedFirstFieldsSpaceNotInOldSpace
		 || (toFinger < top));
		if ((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
			if ((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) {
				if (!previousPin) {
					previousPin = objOop;
					startOfPreviousPin = /* startOfObject: */
							((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) == (numSlotsMask())
								? objOop - BaseHeaderSize
								: objOop);
				}
			}
			else {
				bytes = bytesInBody(objOop);
				while ((toFinger <= startOfPreviousPin)
				 && ((bytes != ((availableSpace = startOfPreviousPin - toFinger)))
				 && ((bytes + (16)) > availableSpace))) {
					if (availableSpace > 0) {
						/* begin addFreeChunkWithBytes:at: */
						totalFreeOldSpace += availableSpace;
						freeChunkWithBytesat(availableSpace, toFinger);
					}
					do {
						assert((isMarked(previousPin))
						 && (isPinned(previousPin)));

						/* begin unmarkPinned: */
						if (((longAt((void *)(previousPin))) & (classIndexMask())) == (segmentBridgePun())) {
							assert(isMarked(previousPin));
						}
						else {
							/* begin setIsMarkedOf:to: */
							assert(!(isFreeObject(previousPin)));
							byteAtput((void *)(previousPin + (markBitsByteOffset())),(byteAt((void *)(previousPin + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));

							/* begin notePinned: */
							assert(isPinned(previousPin));
							assert(!((isSegmentBridge(previousPin))));
							seg = segmentContainingObj(previousPin);
							(seg->containsPinned = 1);
						}
						toFinger = addressAfter(previousPin);

						/* begin objectStartingAt: */
						numSlots = byteAt((void *)(toFinger + (numSlotsFieldByteOffset())));
						previousPin = (numSlots == (numSlotsMask())
									? toFinger + BaseHeaderSize
									: toFinger);
					} while((((byteAt((void *)(previousPin + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)
						 && ((((byteAt((void *)(previousPin + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)
						 && (previousPin < objOop)));

					/* Now previousPin is either equal to o or mobile.
					   Move it to the next pinned object below o */
					while (!((previousPin >= objOop)
					 || ((((byteAt((void *)(previousPin + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)
					 && (((byteAt((void *)(previousPin + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)))) {
						previousPin = oldSpaceObjectAfter(previousPin);
					}
					if (previousPin >= objOop) {
						previousPin = null;
						startOfPreviousPin = 0;
					}
					else {
						startOfPreviousPin = /* startOfObject: */
								((byteAt((void *)(previousPin + (numSlotsFieldByteOffset())))) == (numSlotsMask())
									? previousPin - BaseHeaderSize
									: previousPin);
					}
				}

				/* The object does not fit in the space between toFinger and previousPin.
				   Move toFinger up to point at the first unmarked or mobile object after
				   previousPin, or, if previousPin is contiguous with o, to the start of this
				   object.  Update previousPin to be the next pinned object above toFInger
				   and below this object, or nil if no such pinned object exists.
				   Any unfillable gaps between adjacent pinned objects will be freed. */
				firstField = longAt((void *)(top));

				/* begin copyAndUnmarkObject:to:bytes:firstField: */
				/* begin setIsMarkedOf:to: */
				assert(!(isFreeObject(objOop)));
				byteAtput((void *)(objOop + (markBitsByteOffset())),(byteAt((void *)(objOop + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
				numSlots = byteAt((void *)(objOop + (numSlotsFieldByteOffset())));
				destObj = (numSlots == (numSlotsMask())
							? toFinger + BaseHeaderSize
							: toFinger);
				start = /* startOfObject:given: */
						(numSlots == (numSlotsMask())
							? objOop - BaseHeaderSize
							: objOop);

				/* memmove must be used since the ranges may overlap. */
				memmove(((void *)toFinger), ((void *)start), bytes);

				/* begin storePointerUnchecked:ofObject:withValue: */
				assert((isNonImmediate(destObj))
				 && (!(isForwarded(destObj))));
				assert(validStorePointerUncheckedArgs(0, destObj, firstField));
				longAtput((void *)((destObj + BaseHeaderSize) + (0U << (shiftForWord()))),firstField);
				toFinger += bytes;
				if (((top += BytesPerOop)) >= ((savedFirstFieldsSpace.limit))) {
					assert(((savedFirstFieldsSpace.top)) == (top - BytesPerOop));
					assert(nextObj == objectAfterLastMobileObject);
					if (!previousPin) {
						previousPin = nextObj;
						startOfPreviousPin = /* startOfObject: */
								((byteAt((void *)(nextObj + (numSlotsFieldByteOffset())))) == (numSlotsMask())
									? nextObj - BaseHeaderSize
									: nextObj);
					}

					/* Create a free object for firstFreeObject to be set to on the next pass, but
					   do not link it into the free tree as it will be written over in that next pass. */
					if (toFinger < startOfPreviousPin) {
						firstFreeObject = initFreeChunkWithBytesat(startOfPreviousPin - toFinger, toFinger);
					}
					else {
						firstFreeObject = previousPin;
					}
					return 0;
				}
			}
		}
		prevPrevObj = prevObj;
		prevObj = objOop;
		objOop = nextObj;
	}
	freeFromupTonextObject(toFinger, endOfMemory, (!(previousPin)
		? (!(objectAfterLastMobileObject)
				? objectAfter(firstFreeObject)
				: objectAfterLastMobileObject)
		: previousPin));

	/* begin coalesceFrom: */
	next = 0;
	if (toFinger >= endOfMemory) {
		goto l2;
	}

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(toFinger + (numSlotsFieldByteOffset())));
	obj = (numSlots == (numSlotsMask())
				? toFinger + BaseHeaderSize
				: toFinger);
	while (1) {
		next = oldSpaceObjectAfter(obj);
		if (!(next < endOfMemory)) break;
		if ((((longAt((void *)(obj))) & (classIndexMask())) == (isFreeObjectClassIndexPun()))
		 && (((longAt((void *)(next))) & (classIndexMask())) == (isFreeObjectClassIndexPun()))) {
			objBytes = bytesInBody(obj);
			nextBytes = bytesInBody(next);
			unlinkFreeChunkchunkBytes(obj, objBytes);
			unlinkFreeChunkchunkBytes(next, nextBytes);
			obj = freeChunkWithBytesat(objBytes + nextBytes, /* startOfObject: */
					((byteAt((void *)(obj + (numSlotsFieldByteOffset())))) == (numSlotsMask())
						? obj - BaseHeaderSize
						: obj));
		}
		else {
			obj = next;
		}
	}
	/* end coalesceFrom: */
l2:
	return 1;
}
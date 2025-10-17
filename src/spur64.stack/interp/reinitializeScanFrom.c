/* Extracted from interp.c:47884 (function reinitializeScanFrom). */

/*	Search for firstFreeObject and firstMobileObject from initialObject, which
	is the
	hiddenRootsObject on the first pass, and the objectAfterLastMobileObject
	on subsequent passes). */

	/* SpurPlanningCompactor>>#reinitializeScanFrom: */

static NoDbgRegParms void
reinitializeScanFrom(sqInt initialObject)
{
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt objOop;
    sqInt prevObj;
    sqInt prevPrevObj;

	firstFreeObject = scanForFirstFreeAndFirstMobileObjectFrom(initialObject);
	if (firstFreeObject) {
		mobileStart = /* startOfObject: */
				((byteAt((void *)(firstFreeObject + (numSlotsFieldByteOffset())))) == (numSlotsMask())
					? firstFreeObject - BaseHeaderSize
					: firstFreeObject);
	}
	if (objectAfterLastMobileObject) {
		/* begin allOldSpaceEntitiesFrom:to:do: */
		assert((isNonImmediate(firstFreeObject))
		 && (isInSegments(firstFreeObject)));
		assert((isNonImmediate(objectAfterLastMobileObject))
		 && (isInSegments(objectAfterLastMobileObject)));
		prevPrevObj = (prevObj = null);
		objOop = firstFreeObject;
		while (1) {
			assert((objOop % (allocationUnit())) == 0);
			if (!(oopisLessThanOrEqualTo(objOop, objectAfterLastMobileObject))) break;
			assert((long64At((void *)(objOop))) != 0);
			if (!((((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)
				 || ((!(((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))
				 || (objectAfterLastMobileObject == objOop)))) {
				/* begin setIsMarkedOf:to: */
				assert(!(isFreeObject(objOop)));
				byteAtput((void *)(objOop + (markBitsByteOffset())),(byteAt((void *)(objOop + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
			}
			prevPrevObj = prevObj;
			prevObj = objOop;

			/* begin objectAfter:limit: */
			followingWordAddress = addressAfter(objOop);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
				objOop = endOfMemory;
				goto l1;
			}
			followingWord = longAt((void *)(followingWordAddress));
			objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
						? followingWordAddress + BaseHeaderSize
						: followingWordAddress);
			/* end objectAfter:limit: */
l1:;
		}
		firstMobileObject = objectAfterLastMobileObject;
	}
}
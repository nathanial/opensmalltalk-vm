/* Extracted from interp.c:47884 (function reinitializeScanFrom). */

static NoDbgRegParms void
reinitializeScanFrom(sqInt initialObject)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt objOop;
    sqInt prevObj;
    sqInt prevPrevObj;

	GIV(firstFreeObject) = scanForFirstFreeAndFirstMobileObjectFrom(initialObject);
	if (GIV(firstFreeObject)) {
		GIV(mobileStart) = /* startOfObject: */
				((byteAt((void *)(GIV(firstFreeObject) + (numSlotsFieldByteOffset())))) == (numSlotsMask())
					? GIV(firstFreeObject) - BaseHeaderSize
					: GIV(firstFreeObject));
	}
	if (GIV(objectAfterLastMobileObject)) {
		/* begin allOldSpaceEntitiesFrom:to:do: */
		assert((isNonImmediate(GIV(firstFreeObject)))
		 && (isInSegments(GIV(firstFreeObject))));
		assert((isNonImmediate(GIV(objectAfterLastMobileObject)))
		 && (isInSegments(GIV(objectAfterLastMobileObject))));
		prevPrevObj = (prevObj = null);
		objOop = GIV(firstFreeObject);
		while (1) {
			assert((objOop % (allocationUnit())) == 0);
			if (!(oopisLessThanOrEqualTo(objOop, GIV(objectAfterLastMobileObject)))) break;
			assert((long64At((void *)(objOop))) != 0);
			if (!((((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)
				 || ((!(((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0))
				 || (GIV(objectAfterLastMobileObject) == objOop)))) {
				/* begin setIsMarkedOf:to: */
				assert(!(isFreeObject(objOop)));
				byteAtput((void *)(objOop + (markBitsByteOffset())),(byteAt((void *)(objOop + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));
			}
			prevPrevObj = prevObj;
			prevObj = objOop;

			/* begin objectAfter:limit: */
			followingWordAddress = addressAfter(objOop);
			if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
				objOop = GIV(endOfMemory);
				goto l1;
			}
			followingWord = longAt((void *)(followingWordAddress));
			objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
						? followingWordAddress + BaseHeaderSize
						: followingWordAddress);
			/* end objectAfter:limit: */
l1:;
		}
		GIV(firstMobileObject) = GIV(objectAfterLastMobileObject);
	}
}
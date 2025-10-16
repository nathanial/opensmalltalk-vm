/* Extracted from interp.c:48886 (function validRelocationPlanInPass). */

static NoDbgRegParms sqInt
validRelocationPlanInPass(sqInt onePass)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt destination;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt nMobiles;
    sqInt objOop;
    sqInt prevObj;
    sqInt prevPrevObj;
    usqInt toFinger;

	nMobiles = 0;
	toFinger = GIV(mobileStart);
	GIV(anomaly) = null;

	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(GIV(firstMobileObject)));
	prevPrevObj = (prevObj = null);
	objOop = GIV(firstMobileObject);
	while (1) {
		assert((objOop % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOop, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objOop))) != 0);
		if ((byteAt((void *)(objOop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) {
			if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift())))) {
				nMobiles += 1;
				destination = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
				if (!(destination >= toFinger)) {
					GIV(anomaly) = objOop;
					return 1;
				}
				toFinger += bytesInBody(objOop);
				if (oopisGreaterThan(objOop, GIV(lastMobileObject))) {
					GIV(anomaly) = objOop;
					return 2;
				}
				if (objOop == GIV(lastMobileObject)) {
					return ((((((GIV(savedFirstFieldsSpace).top)) + BytesPerOop) - ((GIV(savedFirstFieldsSpace).start))) / BytesPerOop) == nMobiles
							? 0
							: 3);
				}
			}
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

	/* N.B. written this way so that if there are no mobiles the expression evaluates to 0 in Smalltalk /and/ in C unsigned arithmetic. */
	return ((((((GIV(savedFirstFieldsSpace).top)) + BytesPerOop) - ((GIV(savedFirstFieldsSpace).start))) / BytesPerOop) == nMobiles
			? 0
			: 4);
}
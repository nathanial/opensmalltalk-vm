/* Extracted from interp.c:47642 (function freeFromupTonextObject). */

/*	Free from toFinger up to limit, dealing with possible intervening pinned
	objects. 
 */

	/* SpurPlanningCompactor>>#freeFrom:upTo:nextObject: */

static NoDbgRegParms void
freeFromupTonextObject(usqInt initialToFinger, usqInt limit, sqInt nextObject)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt numSlots;
    sqInt obj;
    usqInt objStart;
    SpurSegmentInfo *seg;
    usqInt toFinger;

	toFinger = initialToFinger;
	objStart = /* startOfObject: */
			((byteAt((void *)(nextObject + (numSlotsFieldByteOffset())))) == (numSlotsMask())
				? nextObject - BaseHeaderSize
				: nextObject);
	if (toFinger < objStart) {
		/* begin addFreeChunkWithBytes:at: */
		totalFreeOldSpace += objStart - toFinger;
		freeChunkWithBytesat(objStart - toFinger, toFinger);
	}
	toFinger = objStart;
	while (objStart < limit) {
		/* begin objectStartingAt: */
		numSlots = byteAt((void *)(objStart + (numSlotsFieldByteOffset())));
		obj = (numSlots == (numSlotsMask())
					? objStart + BaseHeaderSize
					: objStart);
		if ((((byteAt((void *)(obj + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))) != 0)
		 && (((byteAt((void *)(obj + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) != 0)) {
			/* begin unmarkPinned: */
			if (((longAt((void *)(obj))) & (classIndexMask())) == (segmentBridgePun())) {
				assert(isMarked(obj));
			}
			else {
				/* begin setIsMarkedOf:to: */
				assert(!(isFreeObject(obj)));
				byteAtput((void *)(obj + (markBitsByteOffset())),(byteAt((void *)(obj + (markBitsByteOffset())))) & (0xFF - (1U << (markedBitByteShift()))));

				/* begin notePinned: */
				assert(isPinned(obj));
				assert(!((isSegmentBridge(obj))));
				seg = segmentContainingObj(obj);
				(seg->containsPinned = 1);
			}
			if (toFinger < objStart) {
				/* begin addFreeChunkWithBytes:at: */
				totalFreeOldSpace += objStart - toFinger;
				freeChunkWithBytesat(objStart - toFinger, toFinger);
			}
			toFinger = (objStart = addressAfter(obj));
		}
		else {
			objStart = addressAfter(obj);
		}
	}
	if (limit > toFinger) {
		/* begin addFreeChunkWithBytes:at: */
		totalFreeOldSpace += limit - toFinger;
		freeChunkWithBytesat(limit - toFinger, toFinger);
	}
}
/* Extracted from interp.c:48086 (function unmarkObjectsFromFirstFreeObject). */

/*	Sweep the final immobile heap, freeing and coalescing unmarked and free
        objects, and unmarking all marked objects up to the end of memory. */

/* SpurPlanningCompactor>>#unmarkObjectsFromFirstFreeObject */

static void unmarkObjectsFromFirstFreeObject(void) {
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt freeBytes;
  sqInt objOop;
  sqInt prevObj;
  sqInt prevPrevObj;
  SpurSegmentInfo *seg;
  usqInt startOfFree;

  startOfFree = 0;
  freeBytes = 0;

  /* begin allOldSpaceEntitiesFrom:do: */
  assert(isOldObject(firstFreeObject));
  prevPrevObj = (prevObj = null);
  objOop = firstFreeObject;
  while (1) {
    assert((objOop % (allocationUnit())) == 0);
    if (!(oopisLessThan(objOop, endOfMemory)))
      break;
    assert((long64At((void *)(objOop))) != 0);
    if ((byteAt((void *)(objOop + (markBitsByteOffset())))) &
        (1U << (markedBitByteShift()))) {
      if (startOfFree) {
        /* begin addFreeChunkWithBytes:at: */
        totalFreeOldSpace += freeBytes;
        freeChunkWithBytesat(freeBytes, startOfFree);
        startOfFree = null;
        freeBytes = 0;
      }
      if ((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
          (1U << (pinnedBitByteShift()))) {
        /* begin unmarkPinned: */
        if (((longAt((void *)(objOop))) & (classIndexMask())) ==
            (segmentBridgePun())) {
          assert(isMarked(objOop));
        } else {
          /* begin setIsMarkedOf:to: */
          assert(!(isFreeObject(objOop)));
          byteAtput((void *)(objOop + (markBitsByteOffset())),
                    (byteAt((void *)(objOop + (markBitsByteOffset())))) &
                        (0xFF - (1U << (markedBitByteShift()))));

          /* begin notePinned: */
          assert(isPinned(objOop));
          assert(!((isSegmentBridge(objOop))));
          seg = segmentContainingObj(objOop);
          (seg->containsPinned = 1);
        }
      } else {
        /* begin setIsMarkedOf:to: */
        assert(!(isFreeObject(objOop)));
        byteAtput((void *)(objOop + (markBitsByteOffset())),
                  (byteAt((void *)(objOop + (markBitsByteOffset())))) &
                      (0xFF - (1U << (markedBitByteShift()))));
      }
    } else {
      if (!startOfFree) {
        startOfFree = /* startOfObject: */
            ((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) ==
                     (numSlotsMask())
                 ? objOop - BaseHeaderSize
                 : objOop);
      }
      freeBytes += bytesInBody(objOop);
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
    objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                      (numSlotsMask())
                  ? followingWordAddress + BaseHeaderSize
                  : followingWordAddress);
    /* end objectAfter:limit: */
  l1:;
  }
  if (startOfFree) {
    /* begin addFreeChunkWithBytes:at: */
    totalFreeOldSpace += freeBytes;
    freeChunkWithBytesat(freeBytes, startOfFree);
  }
}
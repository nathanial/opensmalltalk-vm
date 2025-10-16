/* Extracted from interp.c:32052 (function checkHeapFreeSpaceIntegrity). */

/*	Perform an integrity/leak check using the heapMap. Assume
        clearLeakMapAndMapAccessibleFreeSpace has set a bit at each free chunk's
        header. Scan all objects in the heap checking that no pointer points
        to a free chunk and that all free chunks that refer to others refer to
        marked chunks. Answer if all checks pass. */

/* SpurMemoryManager>>#checkHeapFreeSpaceIntegrity */

static sqInt checkHeapFreeSpaceIntegrity(void) {
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
    if (freeLists[i]) {
      if (!(heapMapAtWord(pointerForOop(freeLists[i])))) {
        fprintf(transcript, "leak in free list %d to non-free %p\n", ((int)i),
                ((void *)(freeLists[i])));
        ok = 0;
      }
    }
  }

  /* Excuse the duplication but performance is at a premium and we avoid
     some tests by splitting the newSpace and oldSpace enumerations. */

  /* begin allNewSpaceEntitiesDo: */
  prevPrevObj = (prevObj = null);

  /* After a scavenge eden is empty, futureSpace is empty, and all newSpace
     objects are in pastSpace.  Objects are allocated in eden.  So enumerate
     only pastSpace and eden. */
  assert((((pastSpace).start)) < (((eden).start)));
  start = /* startAddressForBridgedHeapEnumeration */
      (pastSpaceStart > (((pastSpace).start))
           ? ((pastSpace).start)
           : (freeStart > (((eden).start)) ? ((eden).start) : oldSpaceStart));
  if (start > freeStart) {
    goto l2;
  }

  /* begin bridgePastSpaceAndEden */
  if (pastSpaceStart < (((eden).start))) {
    if ((pastSpaceStart + BaseHeaderSize) == (((eden).start))) {
      hackSlimBridgeToat(objectStartingAt(((eden).start)), pastSpaceStart);

      /* And carefully check the assumption */
      assert((objectAfterMaybeSlimBridgelimit(
                 objectInPastSpaceBefore(pastSpaceStart), nilObj)) ==
             (objectStartingAt(((eden).start))));
    } else {
      initSegmentBridgeWithBytesat((((eden).start)) - pastSpaceStart,
                                   pastSpaceStart);
    }
  }

  /* begin objectStartingAt: */
  numSlots = byteAt((void *)(start + (numSlotsFieldByteOffset())));
  objOop = (numSlots == (numSlotsMask()) ? start + BaseHeaderSize : start);
  while (oopisLessThan(objOop, freeStart)) {
    if (((longAt((void *)(objOop))) & (classIndexMask())) ==
        (isFreeObjectClassIndexPun())) {
      fprintf(transcript, "young object %p is free\n", ((void *)objOop));
      ok = 0;
    } else {
      if (objOop != freeSpaceCheckOopToIgnore) {
        toDoLimit = (numPointerSlotsOf(objOop)) - 1;
        for (fi = 0; fi <= toDoLimit; fi += 1) {
          fieldOop = fetchPointerofObject(fi, objOop);
          if ((!(fieldOop & (tagMask())))) {
            if (heapMapAtWord(pointerForOop(fieldOop))) {
              fprintf(transcript, "object leak in %p @ %d = %p is free\n",
                      ((void *)objOop), ((int)fi), ((void *)fieldOop));
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
    if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
      objOop = freeStart;
      goto l1;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOop =
        ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                 (numSlotsMask())
             ? ((oopisLessThan(objOop, oldSpaceStart)) &&
                        ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
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
  assert(isOldObject(nilObj));
  prevPrevObj = (prevObj = null);
  objOop = nilObj;
  while (1) {
    assert((objOop % (allocationUnit())) == 0);
    if (!(oopisLessThan(objOop, endOfMemory)))
      break;
    assert((long64At((void *)(objOop))) != 0);
    if (((longAt((void *)(objOop))) & (classIndexMask())) ==
        (isFreeObjectClassIndexPun())) {
      if (!(heapMapAtWord(pointerForOop(objOop)))) {
        fprintf(transcript, "leak in free chunk %p is unmapped?! \n",
                ((void *)objOop));
        ok = 0;
      }
      fieldOop = fetchPointerofObject(0U, objOop);
      if ((fieldOop != 0) && ((heapMapAtWord(pointerForOop(fieldOop))) == 0)) {
        fprintf(transcript, "leak in free chunk %p @ 0 = %p is unmapped\n",
                ((void *)objOop), ((void *)fieldOop));
        ok = 0;
      }
      chunkBytes = bytesInBody(objOop);

      /* begin isLilliputianSize: */
      assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
      if (!(chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */))) {
        fieldOop = fetchPointerofObject(1U, objOop);
        if ((fieldOop != 0) &&
            ((heapMapAtWord(pointerForOop(fieldOop))) == 0)) {
          fprintf(transcript, "leak in free chunk %p @ 0 = %p is unmapped\n",
                  ((void *)objOop), ((void *)fieldOop));
          ok = 0;
        }
      }
      if ((bytesInBody(objOop)) >=
          0x200 /* (numFreeLists * #allocationUnit) */) {
        for (fi = 2 /* freeChunkParentIndex */;
             fi <= 4 /* freeChunkLargerIndex */; fi += 1) {
          fieldOop = fetchPointerofObject(fi, objOop);
          if ((fieldOop != 0) &&
              ((heapMapAtWord(pointerForOop(fieldOop))) == 0)) {
            fprintf(transcript, "leak in free chunk %p @ %d = %p is unmapped\n",
                    ((void *)objOop), ((int)fi), ((void *)fieldOop));
            ok = 0;
          }
        }
      }
      total += bytesInBody(objOop);
    } else {
      if (objOop != freeSpaceCheckOopToIgnore) {
        toDoLimit1 = (numPointerSlotsOf(objOop)) - 1;
        for (fi = 0; fi <= toDoLimit1; fi += 1) {
          if ((!((longAt((void *)(objOop))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
            assert(fi == 0);
            fieldOop = fetchPointerofObject(fi, objOop);
          } else {
            fieldOop = fetchPointerofObject(fi, objOop);
          }

          /* We keep #fetchPointer:ofObject: API here for assertions */
          if ((!(fieldOop & (tagMask())))) {
            if (heapMapAtWord(pointerForOop(fieldOop))) {
              fprintf(transcript, "object leak in %p @ %d = %p is free\n",
                      ((void *)objOop), ((int)fi), ((void *)fieldOop));
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
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objOop = endOfMemory;
      goto l3;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                      (numSlotsMask())
                  ? followingWordAddress + BaseHeaderSize
                  : followingWordAddress);
    /* end objectAfter:limit: */
  l3:;
  }
  if (total != totalFreeOldSpace) {
    fprintf(transcript, "incorrect totalFreeOldSpace; expected %d found %d\n",
            ((int)totalFreeOldSpace), ((int)total));
    ok = 0;
  }
  return ok;
}
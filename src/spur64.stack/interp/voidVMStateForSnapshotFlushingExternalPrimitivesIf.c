/* Extracted from interp.c:65668 (function
 * voidVMStateForSnapshotFlushingExternalPrimitivesIf). */

/* LRPCheck */

/*	Make sure that all VM state that affects the heap contents is voided so
        that the heap is
        ready to be snapshotted. If flushExtPrims is true, flush references to
        external primitives in methods. Answer the activeContext that should be
        stored in the snapshot. */

/* StackInterpreter>>#voidVMStateForSnapshotFlushingExternalPrimitivesIf: */

static NoDbgRegParms sqInt
voidVMStateForSnapshotFlushingExternalPrimitivesIf(sqInt flushExtPrims) {
  sqInt activeContext;
  sqInt address;
  sqInt classIndex;
  sqInt fmt;
  sqInt followingWord;
  usqInt followingWordAddress;
  usqInt numSlots;
  sqInt objSqInt;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt startObject;

  activeContext = divorceAllFrames();

  /* begin bereaveAllMarriedContextsForSnapshotFlushingExternalPrimitivesIf: */
  /* begin allObjectsDo: */
  address = /* startAddressForBridgedHeapEnumeration */
      (pastSpaceStart > (((pastSpace).start))
           ? ((pastSpace).start)
           : (freeStart > (((eden).start)) ? ((eden).start) : oldSpaceStart));

  /* begin objectStartingAt: */
  numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
  startObject =
      (numSlots == (numSlotsMask()) ? address + BaseHeaderSize : address);

  /* begin allEntitiesFrom:do: */
  prevPrevObj = (prevObj = null);
  objSqInt = startObject;
  enableObjectEnumerationFrom(startObject);
  while (1) {
    assert((objSqInt % (allocationUnit())) == 0);
    if (!(oopisLessThan(objSqInt, endOfMemory)))
      break;
    assert((long64At((void *)(objSqInt))) != 0);

    /* begin isEnumerableObject: */
    classIndex = (longAt((void *)(objSqInt))) & (classIndexMask());
    assert((classIndex == (segmentBridgePun())) ||
           ((classIndex == (isForwardedObjectClassIndexPun())) ||
            (((long64At((void *)(objSqInt))) != 0) &&
             (classIndex < (numClassTablePages * (classTablePageSize()))))));
    if (classIndex >= (isForwardedObjectClassIndexPun())) {
      fmt = (byteAt((void *)(objSqInt + (formatFieldByteOffset())))) &
            (formatMask());
      if ((fmt == (indexablePointersFormat())) &&
          (((longAt((void *)(objSqInt))) & (classIndexMask())) ==
           ClassMethodContextCompactIndex)) {
        makeContextSnapshotSafe(objSqInt);
      }

      /* Clean out external functions from compiled methods */
      if (flushExtPrims && (fmt >= (firstCompiledMethodFormat()))) {
        flushExternalPrimitiveOf(objSqInt);
      }
    }
    prevPrevObj = prevObj;
    prevObj = objSqInt;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(objSqInt);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objSqInt = endOfMemory;
      goto l1;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objSqInt =
        ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                 (numSlotsMask())
             ? ((oopisLessThan(objSqInt, oldSpaceStart)) &&
                        ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
                    ? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
                    : followingWordAddress + BaseHeaderSize)
             : followingWordAddress);
    /* end objectAfterMaybeSlimBridge:limit: */
  l1:
    assert(oopisGreaterThan(objSqInt, prevObj));
  }
  return activeContext;
}
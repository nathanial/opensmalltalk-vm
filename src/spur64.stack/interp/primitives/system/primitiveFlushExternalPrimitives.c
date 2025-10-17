/* Extracted from interp.c:16385 (function primitiveFlushExternalPrimitives). */

/*	Primitive. Flush all the existing external primitives in the image thus
        forcing a reload on next invocation.
 */

/* InterpreterPrimitives>>#primitiveFlushExternalPrimitives */

static void primitiveFlushExternalPrimitives(void) {
  sqInt address;
  sqInt classIndex;
  sqInt followingWord;
  usqInt followingWordAddress;
  usqInt numSlots;
  sqInt obj;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt startObject;

  /* begin flushExternalPrimitives */
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
  obj = startObject;
  enableObjectEnumerationFrom(startObject);
  while (1) {
    assert((obj % (allocationUnit())) == 0);
    if (!(oopisLessThan(obj, endOfMemory)))
      break;
    assert((long64At((void *)(obj))) != 0);

    /* begin isEnumerableObject: */
    classIndex = (longAt((void *)(obj))) & (classIndexMask());
    assert((classIndex == (segmentBridgePun())) ||
           ((classIndex == (isForwardedObjectClassIndexPun())) ||
            (((long64At((void *)(obj))) != 0) &&
             (classIndex < (numClassTablePages * (classTablePageSize()))))));
    if (classIndex >= (isForwardedObjectClassIndexPun())) {
      if (((byteAt((void *)(obj + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstCompiledMethodFormat())) {
        flushExternalPrimitiveOf(obj);
      }
    }
    prevPrevObj = prevObj;
    prevObj = obj;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(obj);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      obj = endOfMemory;
      goto l1;
    }
    followingWord = longAt((void *)(followingWordAddress));
    obj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                   (numSlotsMask())
               ? ((oopisLessThan(obj, oldSpaceStart)) &&
                          ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
                      ? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
                      : followingWordAddress + BaseHeaderSize)
               : followingWordAddress);
    /* end objectAfterMaybeSlimBridge:limit: */
  l1:
    assert(oopisGreaterThan(obj, prevObj));
  }

  /* begin flushMethodCache */
  memset(methodCache, 0, MethodCacheSize * (sizeof(methodCache[0])));

  /* this for primitiveExternalMethod */
  lastMethodCacheProbeWrite = 0;

  /* begin flushAtCache */
  memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
  memset(externalPrimitiveTable, 0,
         MaxExternalPrimitiveTableSize * (sizeof(externalPrimitiveTable[0])));
  externalPrimitiveTableFirstFreeIndex = 0;
}
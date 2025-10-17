/* Extracted from interp.c:70634 (function primitiveUnloadModule). */

/*	Primitive. Unload the module with the given name.
        Reloading of the module will happen *later* automatically, when a
        function from it is called. This is forced by invalidating all external
        primitive methods and activations in flushExternalPrimitives.
        N.B. since this is most likely a development time activity we don't care
        about performance. */

/* StackInterpreterPrimitives>>#primitiveUnloadModule */

static void primitiveUnloadModule(void) {
  sqInt address;
  sqInt classIndex;
  sqInt fmt;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt moduleLength;
  sqInt moduleName;
  usqInt numSlots;
  sqInt obj;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt startObject;

  moduleName = longAt(stackPointer);
  if (!(/* isBytes: */
        ((!(moduleName & (tagMask())))) &&
        (((byteAt((void *)(moduleName + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstByteFormat())))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

  /* begin numBytesOfBytes: */
  fmt = (byteAt((void *)(moduleName + (formatFieldByteOffset())))) &
        (formatMask());
  assert(fmt >= (firstByteFormat()));
  moduleLength =
      ((((
          assert((classIndexOf(moduleName)) >
                 (isForwardedObjectClassIndexPun())),
          numSlotsOf(moduleName)))
        << (shiftForWord()))) -
      (fmt & 7);
  if (!(ioUnloadModuleOfLength(oopForPointer(firstIndexableField(moduleName)),
                               moduleLength))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  if ( /* object:equalsString:ofSize: */
      (/* isBytes: */
       ((!(moduleName & (tagMask())))) &&
       (((byteAt((void *)(moduleName + (formatFieldByteOffset())))) &
         (formatMask())) >= (firstByteFormat()))) &&
      ((!(((byteAt((void *)(moduleName + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstCompiledMethodFormat()))) &&
       (((numBytesOfBytes(moduleName)) == moduleLength) &&
        ((strncmp("SqueakFFIPrims", firstIndexableField(moduleName),
                  moduleLength)) == 0)))) {
    primitiveCalloutPointer = ((void *)-1);
  }
  forceInterruptCheck();

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

  /* begin pop: */
  stackPointer += 1 * BytesPerWord;
}
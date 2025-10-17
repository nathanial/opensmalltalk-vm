/* Extracted from interp.c:61461 (function readImageFromFileHeapSizeStartingAt).
 */

/* StackInterpreter>>#readImageFromFile:HeapSize:StartingAt: */

size_t readImageFromFileHeapSizeStartingAt(sqImageFile f,
                                           usqInt desiredHeapSize,
                                           squeakFileOffsetType imageOffset) {
  usqInt allocationReserve;
  sqInt anObject;
  sqInt bit;
  sqInt bytes;
  sqInt bytesRead;
  sqInt bytesToShift;
  sqInt classArrayClass;
  sqInt classArrayObj;
  size_t dataSize;
  sqInt firstSegSize;
  sqInt freeOldSpaceInImage;
  sqInt hdrEdenBytes;
  usqInt hdrMaxExtSemTabSize;
  usqInt hdrNumStackPages;
  sqInt headerFlags;
  sqInt headerSize;
  squeakFileOffsetType headerStart;
  sqInt headroom;
  sqInt headroomSqInt;
  usqInt heapSize;
  usqInt heapSizeUsqInt;
  sqInt i;
  usqInt mem;
  usqInt minimumMemory;
  usqInt numSlots;
  sqInt objOop;
  sqInt oop;
  sqInt rawVersion;
  sqInt seed;
  int swapBytes;
  sqInt toDoLimit;
  sqInt toDoLimit1;
  sqInt version;
  int w;
  unsigned short w1;
  sqInt wSqInt;

  rawVersion = 0;
  transcript = stdout;
  metaclassNumSlots = 6;
  classNameIndex = 6;
  version = checkImageVersionFromstartingAtassignRawVersion(f, imageOffset,
                                                            (&rawVersion));
  if (!version) {
    /* begin bailOutOfImageLoad: */
    fprintf(stderr,
            "This ver %d vm cannot read ver %" PRIdSQINT " image file %s\n",
            ((int)68021 /* imageFormatVersion */), rawVersion, getImageName());
  }
  swapBytes = rawVersion != version;
  multipleBytecodeSetsActive = ((version & MultipleBytecodeSetsBitmask) != 0);
  headerStart = (sqImageFilePosition(f)) - 4;

  /* begin getWord32FromFile:swap: */
  w = 0;
  sqImageFileRead((&w), sizeof(int), 1, f);
  headerSize = (swapBytes ? SQ_SWAP_4_BYTES(w) : w);

  /* begin getLongFromFile:swap: */
  wSqInt = 0;
  sqImageFileRead((&wSqInt), sizeof(wSqInt), 1, f);
  dataSize = ((sqInt)((swapBytes ? SQ_SWAP_8_BYTES(wSqInt) : wSqInt)));

  /* begin getLongFromFile:swap: */
  wSqInt = 0;
  sqImageFileRead((&wSqInt), sizeof(wSqInt), 1, f);
  oldImageBaseAddress = (swapBytes ? SQ_SWAP_8_BYTES(wSqInt) : wSqInt);

  /* begin getLongFromFile:swap: */
  wSqInt = 0;
  sqImageFileRead((&wSqInt), sizeof(wSqInt), 1, f);
  anObject = (swapBytes ? SQ_SWAP_8_BYTES(wSqInt) : wSqInt);

  /* begin specialObjectsOop: */
  specialObjectsOop = anObject;
  wSqInt = 0;
  sqImageFileRead((&wSqInt), sizeof(wSqInt), 1, f);
  seed = (swapBytes ? SQ_SWAP_8_BYTES(wSqInt) : wSqInt);

  /* begin lastHash: */
  if (!((lastHash = seed & (identityHashHalfWordMask())))) {
    while (((lastHash = (((usqInt)(ioUTCMicrosecondsNow()))) &
                        (identityHashHalfWordMask()))) == 0) {
    }
  }

  /* begin getLongFromFile:swap: */
  wSqInt = 0;
  sqImageFileRead((&wSqInt), sizeof(wSqInt), 1, f);
  savedWindowSize = (swapBytes ? SQ_SWAP_8_BYTES(wSqInt) : wSqInt);

  /* begin getLongFromFile:swap: */
  wSqInt = 0;
  sqImageFileRead((&wSqInt), sizeof(wSqInt), 1, f);
  headerFlags = (swapBytes ? SQ_SWAP_8_BYTES(wSqInt) : wSqInt);

  /* begin setImageHeaderFlagsFrom: */
  /* so as to preserve unrecognised flags. */
  imageHeaderFlags = headerFlags;
  fullScreenFlag = headerFlags & 1;
  imageFloatsBigEndian = ((!(headerFlags & 2)) ? 1 : 0);

  /* processHasThreadAffinity := headerFlags anyMask: 4. specific to
     CoInterpreterMT flagInterpretedMethods := headerFlags anyMask: 8. specific
     to CoInterpreter */
  preemptionYields = (!(headerFlags & 16));

  /* noThreadingOfGUIThread := headerFlags anyMask: 32. specific to
   * CoInterpreterMT */
  newFinalization = ((headerFlags & 64) != 0);
  sendWheelEvents = ((headerFlags & 128) != 0);
  if ((((sqInt)primitiveDoMixedArithmetic)) < 0) {
    primitiveDoMixedArithmetic = (!(headerFlags & 0x100));
  }

  /* i.e. has it not been set on the command line?
     N.B. flag mask 512 is responded to by the FilePlugin & FileAttributesPlugin
   */
  if ((((sqInt)upscaleDisplayIfHighDPI)) < 0) {
    upscaleDisplayIfHighDPI = (!(headerFlags & 0x400));
  }

  /* begin getWord32FromFile:swap: */
  w = 0;
  sqImageFileRead((&w), sizeof(int), 1, f);
  extraVMMemory = (swapBytes ? SQ_SWAP_4_BYTES(w) : w);

  /* begin getShortFromFile:swap: */
  w1 = 0;
  sqImageFileRead((&w1), sizeof(unsigned short), 1, f);
  hdrNumStackPages =
      (swapBytes ? ((((usqInt)(w1)) >> 8) & 0xFF) | (((w1 & 0xFF) << 8)) : w1);
  numStackPages =
      (desiredNumStackPages
           ? desiredNumStackPages
           : (hdrNumStackPages ? hdrNumStackPages : defaultNumStackPages()));
  desiredNumStackPages = hdrNumStackPages;

  /* begin getShortFromFile:swap: */
  w1 = 0;
  sqImageFileRead((&w1), sizeof(unsigned short), 1, f);
  theUnknownShort =
      (swapBytes ? ((((usqInt)(w1)) >> 8) & 0xFF) | (((w1 & 0xFF) << 8)) : w1);

  /* begin getWord32FromFile:swap: */
  w = 0;
  sqImageFileRead((&w), sizeof(int), 1, f);
  hdrEdenBytes = (swapBytes ? SQ_SWAP_4_BYTES(w) : w);
  bytes =
      (desiredEdenBytes ? desiredEdenBytes
                        : (hdrEdenBytes ? hdrEdenBytes : defaultEdenBytes()));

  /* begin edenBytes: */
  edenBytes = bytes;
  desiredEdenBytes = hdrEdenBytes;

  /* begin getShortFromFile:swap: */
  w1 = 0;
  sqImageFileRead((&w1), sizeof(unsigned short), 1, f);
  hdrMaxExtSemTabSize =
      (swapBytes ? ((((usqInt)(w1)) >> 8) & 0xFF) | (((w1 & 0xFF) << 8)) : w1);
  if (hdrMaxExtSemTabSize) {
    /* begin setMaxExtSemSizeTo: */
    maxExtSemTabSizeSet = 1;
    ioSetMaxExtSemTableSize(hdrMaxExtSemTabSize);
  }

  /* begin getShortFromFile:swap: */
  w1 = 0;
  sqImageFileRead((&w1), sizeof(unsigned short), 1, f);
  the2ndUnknownShort =
      (swapBytes ? ((((usqInt)(w1)) >> 8) & 0xFF) | (((w1 & 0xFF) << 8)) : w1);

  /* begin getLongFromFile:swap: */
  wSqInt = 0;
  sqImageFileRead((&wSqInt), sizeof(wSqInt), 1, f);
  firstSegSize = (swapBytes ? SQ_SWAP_8_BYTES(wSqInt) : wSqInt);

  /* begin firstSegmentSize: */
  firstSegmentSize = firstSegSize;
  allocationReserve = interpreterAllocationReserveBytes();
  minimumMemory = (dataSize + edenBytes) + allocationReserve;

  /* begin getLongFromFile:swap: */
  wSqInt = 0;
  sqImageFileRead((&wSqInt), sizeof(wSqInt), 1, f);
  freeOldSpaceInImage = (swapBytes ? SQ_SWAP_8_BYTES(wSqInt) : wSqInt);

  /* begin initialHeadroom:givenFreeOldSpaceInImage: */
  headroomSqInt =
      (extraVMMemory ? extraVMMemory
                     : (!(growHeadroom) ? 0x1000000 : growHeadroom));
  if (freeOldSpaceInImage >= headroomSqInt) {
    headroom = 0;
    goto l1;
  }
  if (freeOldSpaceInImage >= ((headroomSqInt * 7) / 8)) {
    headroom = headroomSqInt / 8;
    goto l1;
  }
  if (freeOldSpaceInImage >= ((headroomSqInt * 3) / 4)) {
    headroom = headroomSqInt / 4;
    goto l1;
  }
  if (freeOldSpaceInImage >= ((headroomSqInt * 5) / 8)) {
    headroom = (headroomSqInt * 3) / 8;
    goto l1;
  }
  if (freeOldSpaceInImage >= (headroomSqInt / 2)) {
    headroom = headroomSqInt / 2;
    goto l1;
  }
  headroom = headroomSqInt;
  /* end initialHeadroom:givenFreeOldSpaceInImage: */
l1:
  heapSizeUsqInt = ((dataSize + headroom) + edenBytes) +
                   ((headroom > allocationReserve ? 0 : allocationReserve));

  /* begin roundUpHeapSize: */
  bit = (((highBit(heapSizeUsqInt)) - 1) * 3) / 4;
  heapSize =
      (((heapSizeUsqInt & ((1ULL << bit) - 1)) != 0)
           ? (((heapSizeUsqInt | ((1ULL << bit) - 1)) - ((1ULL << bit) - 1))) +
                 (1ULL << bit)
           : heapSizeUsqInt);
  if ((mem = ((usqInt)(allocateMemoryMinimumImageFileHeaderSize(
           heapSize, minimumMemory, f, headerSize))))) {
    setHeapBasememoryLimitendOfMemory(mem, mem + heapSize, mem + dataSize);
  } else {
    insufficientMemoryAvailableError();
  }
  sqImageFileSeek(f, headerStart + headerSize);
  bytesRead = readHeapFromImageFiledataBytes(f, dataSize);
  if (bytesRead != dataSize) {
    unableToReadImageError();
  }
  ensureImageFormatIsUpToDate(swapBytes);
  bytesToShift = oldSpaceStart - oldImageBaseAddress;

  /* begin initializeInterpreter: */
  interpreterProxy = sqGetInterpreterProxy();
  (&interpreterProxy);
  initializeObjectMemory(bytesToShift);

  /* begin checkAssumedCompactClasses */
  /* begin checkCompactIndex:isClass:named: */
  if ((longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                       ((((usqInt)(ClassArray) << (shiftForWord()))))))) !=
      ((/* begin knownClassAtIndex: */
        assert(((ClassArrayCompactIndex >= 1) &&
                (ClassArrayCompactIndex <= (classTablePageSize())))),
        /* fetchPointer:ofObject: */
        longAt((void *)((classTableFirstPage + BaseHeaderSize) +
                        ((((usqInt)(ClassArrayCompactIndex)
                           << (shiftForWord()))))))))) {
    invalidCompactClassError("Array");
  }

  /* begin checkCompactIndex:isClass:named: */
  if ((longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                       ((((usqInt)(ClassLargeNegativeInteger)
                          << (shiftForWord()))))))) !=
      ((/* begin knownClassAtIndex: */
        assert((
            (ClassLargeNegativeIntegerCompactIndex >= 1) &&
            (ClassLargeNegativeIntegerCompactIndex <= (classTablePageSize())))),
        /* fetchPointer:ofObject: */
        longAt((void *)((classTableFirstPage + BaseHeaderSize) +
                        ((((usqInt)(ClassLargeNegativeIntegerCompactIndex)
                           << (shiftForWord()))))))))) {
    invalidCompactClassError("LargeNegativeInteger");
  }

  /* begin checkCompactIndex:isClass:named: */
  if ((longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                       ((((usqInt)(ClassLargePositiveInteger)
                          << (shiftForWord()))))))) !=
      ((/* begin knownClassAtIndex: */
        assert((
            (ClassLargePositiveIntegerCompactIndex >= 1) &&
            (ClassLargePositiveIntegerCompactIndex <= (classTablePageSize())))),
        /* fetchPointer:ofObject: */
        longAt((void *)((classTableFirstPage + BaseHeaderSize) +
                        ((((usqInt)(ClassLargePositiveIntegerCompactIndex)
                           << (shiftForWord()))))))))) {
    invalidCompactClassError("LargePositiveInteger");
  }

  /* begin checkCompactIndex:isClass:named: */
  if ((longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                       ((((usqInt)(ClassFloat) << (shiftForWord()))))))) !=
      ((/* begin knownClassAtIndex: */
        assert(((ClassFloatCompactIndex >= 1) &&
                (ClassFloatCompactIndex <= (classTablePageSize())))),
        /* fetchPointer:ofObject: */
        longAt((void *)((classTableFirstPage + BaseHeaderSize) +
                        ((((usqInt)(ClassFloatCompactIndex)
                           << (shiftForWord()))))))))) {
    invalidCompactClassError("Float");
  }

  /* begin checkCompactIndex:isClass:named: */
  if ((longAt(
          (void *)((specialObjectsOop + BaseHeaderSize) +
                   ((((usqInt)(ClassBlockClosure) << (shiftForWord()))))))) !=
      ((/* begin knownClassAtIndex: */
        assert(((ClassBlockClosureCompactIndex >= 1) &&
                (ClassBlockClosureCompactIndex <= (classTablePageSize())))),
        /* fetchPointer:ofObject: */
        longAt((void *)((classTableFirstPage + BaseHeaderSize) +
                        ((((usqInt)(ClassBlockClosureCompactIndex)
                           << (shiftForWord()))))))))) {
    invalidCompactClassError("BlockClosure");
  }

  /* begin checkCompactIndex:isClass:named: */
  if ((longAt(
          (void *)((specialObjectsOop + BaseHeaderSize) +
                   ((((usqInt)(ClassMethodContext) << (shiftForWord()))))))) !=
      ((/* begin knownClassAtIndex: */
        assert(((ClassMethodContextCompactIndex >= 1) &&
                (ClassMethodContextCompactIndex <= (classTablePageSize())))),
        /* fetchPointer:ofObject: */
        longAt((void *)((classTableFirstPage + BaseHeaderSize) +
                        ((((usqInt)(ClassMethodContextCompactIndex)
                           << (shiftForWord()))))))))) {
    invalidCompactClassError("MethodContext");
  }
  objOop = longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                           ((((usqInt)(ClassByteArray) << (shiftForWord()))))));

  /* begin compactIndexOfClass: */
  assert((rawHashBitsOf(objOop)) != 0);
  classByteArrayCompactIndex =
      (long32At((void *)(objOop + 4))) & (identityHashHalfWordMask());

  /* begin initializeExtraClassInstVarIndices */
  classArrayObj =
      longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                      ((((usqInt)(ClassArray) << (shiftForWord()))))));
  classArrayClass = fetchClassOfNonImm(classArrayObj);

  /* begin numSlotsOf: */
  assert((classIndexOf(classArrayClass)) > (isForwardedObjectClassIndexPun()));

  /* determine actual Metaclass instSize */
  metaclassNumSlots =
      (((numSlots = byteAt(
             (void *)(classArrayClass + (numSlotsFieldByteOffset()))))) ==
               (numSlotsMask())
           ? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(classArrayClass -
                                                            BaseHeaderSize))))
                                  << 8)))))) >>
                 8
           : numSlots);

  /* default */
  thisClassIndex = 5;
  /* begin numSlotsOf: */
  assert((classIndexOf(classArrayClass)) > (isForwardedObjectClassIndexPun()));
  if (((numSlots =
            byteAt((void *)(classArrayClass + (numSlotsFieldByteOffset()))))) ==
      (numSlotsMask())) {
    toDoLimit =
        ((((usqInt)(((sqInt)((usqInt)((longAt(
                                 (void *)(classArrayClass - BaseHeaderSize))))
                             << 8)))))) >>
        8;
  } else {
    toDoLimit = numSlots;
  }
  for (i = (InstanceSpecificationIndex + 1); i <= toDoLimit; i += 1) {
    if ((longAt((void *)((classArrayClass + BaseHeaderSize) +
                         ((((usqInt)((i - 1)) << (shiftForWord()))))))) ==
        classArrayObj) {
      thisClassIndex = i - 1;
    }
  }

  /* default */
  classNameIndex = 6;
  /* begin numSlotsOf: */
  assert((classIndexOf(classArrayObj)) > (isForwardedObjectClassIndexPun()));
  if (((numSlots =
            byteAt((void *)(classArrayObj + (numSlotsFieldByteOffset()))))) ==
      (numSlotsMask())) {
    toDoLimit1 =
        ((((usqInt)((
            (sqInt)((usqInt)((longAt((void *)(classArrayObj - BaseHeaderSize))))
                    << 8)))))) >>
        8;
  } else {
    toDoLimit1 = numSlots;
  }
  for (i = (InstanceSpecificationIndex + 1); i <= toDoLimit1; i += 1) {
    oop = longAt((void *)((classArrayObj + BaseHeaderSize) +
                          ((((usqInt)((i - 1)) << (shiftForWord()))))));
    if (objectequalsString(oop, "Array")) {
      classNameIndex = i - 1;
    }
  }
  method = (newMethod = nilObj);
  methodDictLinearSearchLimit = 8;

  /* begin initialCleanup */
  /* begin flushMethodCache */
  memset(methodCache, 0, MethodCacheSize * (sizeof(methodCache[0])));

  /* this for primitiveExternalMethod */
  lastMethodCacheProbeWrite = 0;

  /* begin flushAtCache */
  memset(atCache, 0, AtCacheTotalSize * (sizeof(atCache[0])));
  memset(externalPrimitiveTable, 0,
         MaxExternalPrimitiveTableSize * (sizeof(externalPrimitiveTable[0])));
  externalPrimitiveTableFirstFreeIndex = 0;

  /* cmd-. as used for Mac but no other OS */
  interruptKeycode = ((8U << 8)) + (((sqInt)'.'));
  while (globalSessionID == 0) {
    globalSessionID = ((time(NULL)) + (ioMSecs())) & 0x7FFFFFFF;
  }
  metaAccessorDepth = -2;
  sHEAFn = ioLoadFunctionFrom("secHasEnvironmentAccess", "SecurityPlugin");
  return dataSize;
}
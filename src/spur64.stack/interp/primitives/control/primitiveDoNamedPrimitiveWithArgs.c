/* Extracted from interp.c:67390 (function primitiveDoNamedPrimitiveWithArgs).
 */

/*	Simulate an primitiveExternalCall invocation (e.g. for the Debugger). Do
        not cache anything.
        e.g. ContextPart>>tryNamedPrimitiveIn: aCompiledMethod for: aReceiver
        withArgs: arguments */

/* StackInterpreterPrimitives>>#primitiveDoNamedPrimitiveWithArgs */

static void primitiveDoNamedPrimitiveWithArgs(void) {
  void (*addr)(void);
  sqInt argumentArray;
  usqInt arraySize;
  sqInt ccIndex;
  usqInt firstBytecode;
  sqInt fmt;
  sqInt functionLength;
  sqInt functionName;
  sqInt index;
  sqInt isArray;
  sqInt methodArg;
  sqInt methodHeader;
  sqInt moduleLength;
  sqInt moduleName;
  usqInt newObj;
  usqInt numBytes;
  usqInt numSlots;
  sqInt primRcvr;
  char *sp;
  sqInt spec;
  sqInt successBoolean;
  sqInt top;
  sqInt valuePointer;

  metaAccessorDepth = -2;

  /* See checkForAndFollowForwardedPrimitiveState */
  argumentArray = longAt(stackPointer);
  methodArg = longAt(stackPointer + (2 * BytesPerWord));
  if (!((/* isArray: */
         ((!(argumentArray & (tagMask())))) &&
         (((byteAt((void *)(argumentArray + (formatFieldByteOffset())))) &
           (formatMask())) == (arrayFormat()))) &&
        (/* isOopCompiledMethod: */
         ((!(methodArg & (tagMask())))) &&
         (((byteAt((void *)(methodArg + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstCompiledMethodFormat()))))) {
    /* primitiveFailFor: */
    primFailCode = -2;
    return;
  }

  arraySize = numSlotsOf(argumentArray);
  if (!(arraySize <= (LargeContextSlots - CtxtTempFrameStart))) {
    /* primitiveFailFor: */
    primFailCode = -2;
    return;
  }

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(methodArg));
  methodHeader = fetchPointerofObject(HeaderIndex, methodArg);
  if (!(((/* begin literalCountOfMethodHeader: */
          assert((((methodHeader) & 7) == 1)),
          /* literalCountOfAlternateHeader: */
          ((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) > 2)) {
    /* primitiveFailFor: */
    primFailCode = -3;
    return;
  }
  spec = fetchPointerofObject(1U, methodArg);

  /* first literal */

  /* begin isInstanceOfClassArray: */
  /* begin is:instanceOf:compactClassIndex: */
  if (((spec & (tagMask())) != 0)) {
    isArray = 0;
    goto l2;
  }

  /* begin isClassOfNonImm:equalTo:compactClassIndex: */
  assert(!(isImmediate(spec)));
  ccIndex = (longAt((void *)(spec))) & (classIndexMask());
  isArray = ClassArrayCompactIndex == ccIndex;
  /* end isInstanceOfClassArray: */
l2:
  if (!(isArray &&
        ((((numSlotsOf(spec))) == 4) &&
         ((/* primitiveIndexOfMethod:header: */
           (((methodHeader & AlternateHeaderHasPrimFlag) != 0)
                ? ((firstBytecode =
                        (methodArg +
                         ((LiteralStart + (((methodHeader >> 3)) &
                                           AlternateHeaderNumLiteralsMask)) *
                          BytesPerOop)) +
                        BaseHeaderSize),
                   (byteAt((void *)(firstBytecode + 1))) +
                       ((((usqInt)((byteAt((void *)(firstBytecode + 2))))
                          << 8))))
                : 0)) == PrimNumberExternalCall)))) {
    /* primitiveFailFor: */
    primFailCode = -3;
    return;
  }
  if (!(((((usqInt)(methodHeader)) >> MethodHeaderArgCountShift) & 15) ==
        arraySize)) {
    /* primitiveFailFor: */
    primFailCode = -2;
    return;
  }
  moduleName = fetchPointerofObject(0U, spec);
  if (moduleName == nilObj) {
    moduleLength = 0;
  } else {
    successBoolean = /* isBytes: */
        ((!(moduleName & (tagMask())))) &&
        (((byteAt((void *)(moduleName + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstByteFormat()));

    /* begin success: */
    if (!successBoolean) {
      if (!primFailCode) {
        primFailCode = 1;
      }
    }

    /* begin numBytesOfBytes: */
    fmt = (byteAt((void *)(moduleName + (formatFieldByteOffset())))) &
          (formatMask());
    assert(fmt >= (firstByteFormat()));
    moduleLength = ((((assert((classIndexOf(moduleName)) >
                              (isForwardedObjectClassIndexPun())),
                       numSlotsOf(moduleName)))
                     << (shiftForWord()))) -
                   (fmt & 7);
  }
  functionName = fetchPointerofObject(1U, spec);
  successBoolean = /* isBytes: */
      ((!(functionName & (tagMask())))) &&
      (((byteAt((void *)(functionName + (formatFieldByteOffset())))) &
        (formatMask())) >= (firstByteFormat()));

  /* begin success: */
  if (!successBoolean) {
    if (!primFailCode) {
      primFailCode = 1;
    }
  }

  /* begin numBytesOfBytes: */
  fmt = (byteAt((void *)(functionName + (formatFieldByteOffset())))) &
        (formatMask());
  assert(fmt >= (firstByteFormat()));
  functionLength = ((((assert((classIndexOf(functionName)) >
                              (isForwardedObjectClassIndexPun())),
                       numSlotsOf(functionName)))
                     << (shiftForWord()))) -
                   (fmt & 7);
  if (primFailCode) {
    /* primitiveFailFor: */
    primFailCode = -3;
    return;
  }
  addr = ioLoadExternalFunctionOfLengthFromModuleOfLengthMetadataInto(
      functionName + BaseHeaderSize, functionLength,
      moduleName + BaseHeaderSize, moduleLength, (&metaAccessorDepth));

  /* N.B. the accessor depth is the second byte of the primitive's metadata;
     the first byte is various flags (currently l.s.b. = use fast C linkage). */
  metaAccessorDepth = (addr ? (((usqInt)(metaAccessorDepth)) >> 8) : -2);
  if (!addr) {
    /* primitiveFailFor: */
    primFailCode = -1;
    return;
  }

  /* begin eeInstantiateClassIndex:format:numSlots: */
  assert((knownClassAtIndex(ClassArrayCompactIndex)) != nilObj);
  assert((arrayFormat()) ==
         (instSpecOfClass(knownClassAtIndex(ClassArrayCompactIndex))));
  newObj = freeStart;
  numBytes = BaseHeaderSize + (4 * BytesPerOop);
  if ((freeStart + numBytes) > scavengeThreshold) {
    if (!needGCFlag) {
      /* begin scheduleScavenge */
      needGCFlag = 1;
      forceInterruptCheck();
    }
    if ((freeStart + numBytes) > (((eden).limit))) {
      error("no room in eden for allocateNewSpaceSlots:format:classIndex:");
      tempOop = 0;
      goto l1;
    }
  }
  long64Atput((void *)(newObj),
              ((((((usqLong)4)) << (numSlotsFullShift()))) +
               ((((usqInt)((arrayFormat())) << (formatShift()))))) +
                  ClassArrayCompactIndex);

  /* for header parsing we put a saturated slot count in the prepended overflow
   * size word */
  assert((numBytes % (allocationUnit())) == 0);
  assert((newObj % (allocationUnit())) == 0);
  freeStart += numBytes;
  tempOop = newObj;
  /* end eeInstantiateClassIndex:format:numSlots: */
l1:

  /* begin popStack */
  valuePointer = (argumentArray = longAt(stackPointer));
  stackPointer += BytesPerWord;
  valuePointer = (argumentArray);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(tempOop)) && (!(isForwarded(tempOop))));
  assert(validStorePointerUncheckedArgs(0, tempOop, valuePointer));
  longAtput((void *)((tempOop + BaseHeaderSize) + (0U << (shiftForWord()))),
            valuePointer);

  /* begin popStack */
  valuePointer = (primRcvr = longAt(stackPointer));
  stackPointer += BytesPerWord;
  valuePointer = (primRcvr);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(tempOop)) && (!(isForwarded(tempOop))));
  assert(validStorePointerUncheckedArgs(1, tempOop, valuePointer));
  longAtput((void *)((tempOop + BaseHeaderSize) + (1U << (shiftForWord()))),
            valuePointer);

  /* begin popStack */
  top = longAt(stackPointer);
  stackPointer += BytesPerWord;
  valuePointer = top;

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(tempOop)) && (!(isForwarded(tempOop))));
  assert(validStorePointerUncheckedArgs(2, tempOop, valuePointer));
  longAtput((void *)((tempOop + BaseHeaderSize) + (2U << (shiftForWord()))),
            valuePointer);

  /* begin popStack */
  top = longAt(stackPointer);
  stackPointer += BytesPerWord;
  valuePointer = top;

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(tempOop)) && (!(isForwarded(tempOop))));
  assert(validStorePointerUncheckedArgs(3, tempOop, valuePointer));
  longAtput((void *)((tempOop + BaseHeaderSize) + (3U << (shiftForWord()))),
            valuePointer);

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), primRcvr);
  stackPointer = sp;
  argumentCount = arraySize;
  for (index = 1; index <= arraySize; index += 1) {
    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord),
              fetchPointerofObject(index - 1, argumentArray));
    stackPointer = sp;
  }

  /* begin callExternalPrimitive: */
  primitiveFunctionPointer = addr;
  dispatchFunctionPointer(addr);
  if (primFailCode) {
    /* begin pop: */
    stackPointer += (arraySize + 1) * BytesPerWord;

    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord),
              fetchPointerofObject(3U, tempOop));
    stackPointer = sp;

    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord),
              fetchPointerofObject(2U, tempOop));
    stackPointer = sp;

    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord),
              fetchPointerofObject(1U, tempOop));
    stackPointer = sp;

    /* begin push: */
    longAtput((sp = stackPointer - BytesPerWord),
              fetchPointerofObject(0U, tempOop));
    stackPointer = sp;
    argumentCount = 3;

    /* Must reset primitiveFunctionPointer for
     * checkForAndFollowForwardedPrimitiveState */
    primitiveFunctionPointer = primitiveDoNamedPrimitiveWithArgs;

    /* Hack.  A nil prim error code (primErrorCode = 1) is interpreted by the
       image as meaning this primitive is not implemented.  So to pass back nil
       as an error code we use -1 to indicate generic failure. */
    if (primFailCode == 1) {
      primFailCode = -1;
    }
  }
}
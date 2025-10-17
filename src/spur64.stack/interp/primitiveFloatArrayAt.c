/* Extracted from interp.c:15534 (function primitiveFloatArrayAt). */

/*	Index the receiver, which must be an indexable non-pointer object, and
        yield a float.
 */

/* InterpreterPrimitives>>#primitiveFloatArrayAt */

static void primitiveFloatArrayAt(void) {
  float aFloat;
  sqLong doubleBits;
  sqInt fmt;
  sqInt index;
  usqInt newFloatObj;
  usqInt newObj;
  usqInt numBytes;
  sqInt numSlots;
  sqInt numSlotsSqInt;
  sqInt oop;
  usqLong rawFloatBits;
  sqInt rcvr;
  usqLong rot;
  char *sp;

  /* begin primitiveSpurFloatArrayAt */
  index = longAt(stackPointer);
  rcvr = longAt(stackPointer + (1 * BytesPerWord));
  if (!((((index) & 7) == 1))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    goto l3;
  }
  if (((rcvr & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    goto l3;
  }
  fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
  index = ((index >> 3)) - 1;
  if (fmt == (sixtyFourBitIndexableFormat())) {
    numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 3;
    if ((((usqInt)index)) < numSlots) {
      doubleBits = long64At(
          (void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 3)))));

      /* begin floatObjectOfBits: */
      if (isSmallFloatValueBits(doubleBits)) {
        rawFloatBits = doubleBits;

        /* begin smallFloatObjectOfBits: */
        assert(isSmallFloatValueBits(rawFloatBits));
        rot = (((rawFloatBits) >> 0x3F) & 1) + ((rawFloatBits << 1));
        if (rot > 1) {
          rot -= (((usqInt)((smallFloatExponentOffset()))
                   << ((smallFloatMantissaBits()) + 1)));
          assert(rot > 0);
        }

        /* a.k.a. ~= +/-0.0 */
        oop = ((rot << (numTagBits()))) + (smallFloatTag());
        goto l2;
      }
      numSlotsSqInt = (sizeof(double)) / BytesPerOop;

      /* begin eeInstantiateSmallClassIndex:format:numSlots: */
      assert((numSlotsSqInt >= 0) &&
             ((knownClassAtIndex(ClassFloatCompactIndex)) != nilObj));
      assert((firstLongFormat()) ==
             (instSpecOfClass(knownClassAtIndex(ClassFloatCompactIndex))));

      /* begin allocateSmallNewSpaceSlots:format:classIndex: */
      assert(numSlotsSqInt < (numSlotsMask()));
      newObj = freeStart;
      numBytes =
          BaseHeaderSize + ((numSlotsSqInt < 1 ? 8 /* allocationUnit */
                                               : numSlotsSqInt * BytesPerOop));
      assert((numBytes % (allocationUnit())) == 0);
      assert((newObj % (allocationUnit())) == 0);
      if ((freeStart + numBytes) > scavengeThreshold) {
        if (!needGCFlag) {
          /* begin scheduleScavenge */
          needGCFlag = 1;
          forceInterruptCheck();
        }
        if ((freeStart + numBytes) > (((eden).limit))) {
          error("no room in eden for "
                "allocateSmallNewSpaceSlots:format:classIndex:");
          newFloatObj = 0;
          goto l1;
        }
      }
      long64Atput((void *)(newObj),
                  ((((((usqLong)numSlotsSqInt)) << (numSlotsFullShift()))) +
                   ((((usqInt)((firstLongFormat())) << (formatShift()))))) +
                      ClassFloatCompactIndex);
      freeStart += numBytes;
      newFloatObj = newObj;
      /* end eeInstantiateSmallClassIndex:format:numSlots: */
    l1:

      /* storeLong64:ofObject:withValue: */
      long64Atput((void *)((newFloatObj + BaseHeaderSize)), doubleBits);
      oop = newFloatObj;
      /* end floatObjectOfBits: */
    l2:

      /* begin methodReturnValue: */
      assert(!((failed())));
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          oop);
      stackPointer = sp;
      goto l3;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    goto l3;
  }
  if ((fmt >= (firstLongFormat())) && (fmt <= ((firstLongFormat()) + 1))) {
    numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 2;
    if ((((usqInt)index)) < numSlots) {
      /* begin fetchFloat32:ofObject: */
      aFloat = singleFloatAt(
          (void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 2)))));
      oop = floatObjectOf(aFloat);

      /* begin methodReturnValue: */
      assert(!((failed())));
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          oop);
      stackPointer = sp;
      goto l3;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    goto l3;
  }

  /* primitiveFailFor: */
  primFailCode = PrimErrBadReceiver;
  /* end primitiveSpurFloatArrayAt */
l3:;
}
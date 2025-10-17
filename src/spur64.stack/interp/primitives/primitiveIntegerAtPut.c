/* Extracted from interp.c:18168 (function primitiveIntegerAtPut). */

/* InterpreterPrimitives>>#primitiveIntegerAtPut */

static void primitiveIntegerAtPut(void) {
  sqInt classFormat;
  sqInt classPointer;
  sqInt fieldIndex;
  sqInt fmt;
  sqInt fmtSqInt;
  sqInt index;
  sqInt numFixed;
  usqInt numSlots;
  sqInt rcvr;
  char *sp;
  sqLong value;
  sqInt valueOop;

  /* begin primitiveSpurIntegerAtPut */
  valueOop = longAt(stackPointer);
  index = longAt(stackPointer + (1 * BytesPerWord));
  rcvr = longAt(stackPointer + (2 * BytesPerWord));
  value = signed64BitValueOf(valueOop);
  if (!((!primFailCode) && ((((index) & 7) == 1)))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    goto l1;
  }
  if (((rcvr & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    goto l1;
  }
  if (
#if IMMUTABILITY
      ((((usqInt)((
            byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >>
        (immutableBitByteShift())) &
       1) != 0
#else
      0
#endif
  ) {
    /* primitiveFailFor: */
    primFailCode = PrimErrNoModification;
    goto l1;
  }
  fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
  index = ((index >> 3)) - 1;
  if (fmt >= (firstByteFormat())) {
    if (fmt >= (firstCompiledMethodFormat())) {
      /* primitiveFailFor: */
      primFailCode = PrimErrUnsupported;
      goto l1;
    }
    if (!((((usqInt)((((value) >> 7)) + 1))) <= 1)) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      goto l1;
    }

    /* begin numBytesOfBytes: */
    fmtSqInt =
        (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
    assert(fmtSqInt >= (firstByteFormat()));
    numSlots = ((((numSlotsOf(rcvr))) << (shiftForWord()))) - (fmtSqInt & 7);
    if ((((usqInt)index)) < numSlots) {
      /* storeByte:ofObject:withValue: */
      byteAtput((void *)((rcvr + BaseHeaderSize) + index), value);

      /* begin methodReturnValue: */
      assert(!((failed())));
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          valueOop);
      stackPointer = sp;
      goto l1;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    goto l1;
  }
  if (fmt >= (firstShortFormat())) {
    if (!((((usqInt)((((value) >> 15)) + 1))) <= 1)) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      goto l1;
    }
    numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 1;
    if ((((usqInt)index)) < numSlots) {
      /* storeShort16:ofObject:withValue: */
      shortAtput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 1)))),
                 value);

      /* begin methodReturnValue: */
      assert(!((failed())));
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          valueOop);
      stackPointer = sp;
      goto l1;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    goto l1;
  }
  if (fmt == (sixtyFourBitIndexableFormat())) {
    numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 3;
    if ((((usqInt)index)) < numSlots) {
      /* storeLong64:ofObject:withValue: */
      long64Atput(
          (void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 3)))),
          value);

      /* begin methodReturnValue: */
      assert(!((failed())));
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          valueOop);
      stackPointer = sp;
      goto l1;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    goto l1;
  }

  /* No range check on value in this case because signed64BitValueOf: performed
   * it above. */
  if (fmt >= (firstLongFormat())) {
    if (!((((usqInt)((((value) >> 0x1F)) + 1))) <= 1)) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      goto l1;
    }
    numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 2;
    if ((((usqInt)index)) < numSlots) {
      /* storeLong32:ofObject:withValue: */
      long32Atput(
          (void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 2)))),
          value);

      /* begin methodReturnValue: */
      assert(!((failed())));
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          valueOop);
      stackPointer = sp;
      goto l1;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    goto l1;
  }
  if ((fmt <= 5 /* lastPointerFormat */) &&
      (/* isIndexableFormat: */
       (fmt >= (arrayFormat())) &&
       ((fmt <= (weakArrayFormat())) ||
        (fmt >= (sixtyFourBitIndexableFormat()))))) {

    numSlots = numSlotsOf(rcvr);
    if (fmt == (arrayFormat())) {
      if ((((usqInt)index)) < numSlots) {
        /* begin storePointer:ofObject:withValue: */
        assert(validStorePointerArgs(index, rcvr, valueOop));
        assert(isNonImmediate(rcvr));
        if (oopisGreaterThanOrEqualTo(rcvr, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(valueOop & (tagMask())))) &&
              (oopisLessThan(valueOop, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(rcvr);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((rcvr + BaseHeaderSize) +
                           ((((usqInt)(index) << (shiftForWord()))))),
                  valueOop);

        /* begin methodReturnValue: */
        assert(!((failed())));
        longAtput(
            (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            valueOop);
        stackPointer = sp;
        goto l1;
      }
    }

    /* begin numFixedSlotsOf: */
    classPointer = fetchClassOfNonImm(rcvr);
    classFormat =
        ((fetchPointerofObject(InstanceSpecificationIndex, classPointer)) >> 3);
    numFixed = classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
    if ((((index + 1) >= numFixed) && ((index + 1) <= numSlots))) {
      fieldIndex = index + numFixed;

      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(fieldIndex, rcvr, valueOop));
      assert(isNonImmediate(rcvr));
      if (oopisGreaterThanOrEqualTo(rcvr, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(valueOop & (tagMask())))) &&
            (oopisLessThan(valueOop, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(rcvr);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((rcvr + BaseHeaderSize) +
                         ((((usqInt)(fieldIndex) << (shiftForWord()))))),
                valueOop);

      /* begin methodReturnValue: */
      assert(!((failed())));
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          valueOop);
      stackPointer = sp;
      goto l1;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    goto l1;
  }

  /* primitiveFailFor: */
  primFailCode = PrimErrBadReceiver;
  /* end primitiveSpurIntegerAtPut */
l1:;
}
/* Extracted from interp.c:69782 (function primitiveSlotAtPut). */

/*	Assign a slot in an object. This numbers all slots from 1, ignoring the
        distinction between
        named and indexed inst vars. In objects with both named and indexed inst
        vars, the named
        inst vars precede the indexed ones. In non-object indexed objects
   (objects that contain bits, not object references) this primitive assigns a
   raw integral value at each slot. */

/* StackInterpreterPrimitives>>#primitiveSlotAtPut */

static void primitiveSlotAtPut(void) {
  sqInt fmt;
  sqInt fmtSqInt;
  sqInt index;
  sqInt newValue;
  usqInt numSlots;
  sqInt rcvr;
  char *sp;
  usqIntptr_t value;
  sqInt valueSqInt;

  newValue = longAt(stackPointer);
  index = longAt(stackPointer + (1 * BytesPerWord));
  rcvr = longAt(stackPointer + (2 * BytesPerWord));
  if (!((((index) & 7) == 1))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  if (((rcvr & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
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
    return;
  }
  fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
  index = ((index >> 3)) - 1;
  if (fmt <= 5 /* lastPointerFormat */) {

    numSlots = numSlotsOf(rcvr);
    if ((((usqInt)index)) < numSlots) {
      if (((longAt((void *)(rcvr))) & (classIndexMask())) ==
          ClassMethodContextCompactIndex) {
        externalInstVarofContextput(index, rcvr, newValue);
      } else {
        storePointerofObjectwithValue(index, rcvr, newValue);
      }

      /* begin pop:thenPush: */
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          newValue);
      stackPointer = sp;
      return;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }

  /* begin positiveMachineIntegerValueOf: */
  if ((((newValue) & 7) == 1)) {
    valueSqInt = (newValue >> 3);
    if (valueSqInt < 0) {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
      value = null;
      goto l1;
    }
    value = valueSqInt;
    goto l1;
  }

  /* don't inline the rare case */
  value = positiveMachineIntegerValueOfObj(newValue);
  /* end positiveMachineIntegerValueOf: */
l1:
  if (primFailCode) {
    primFailCode = PrimErrBadArgument;
    return;
  }
  if (fmt >= (firstByteFormat())) {
    if (fmt >= (firstCompiledMethodFormat())) {
      /* primitiveFailFor: */
      primFailCode = PrimErrUnsupported;
      return;
    }
    if ((((usqInt)value)) > 0xFF) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      return;
    }

    /* begin numBytesOfBytes: */
    fmtSqInt =
        (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
    assert(fmtSqInt >= (firstByteFormat()));
    numSlots = ((((numSlotsOf(rcvr))) << (shiftForWord()))) - (fmtSqInt & 7);
    if ((((usqInt)index)) < numSlots) {
      /* storeByte:ofObject:withValue: */
      byteAtput((void *)((rcvr + BaseHeaderSize) + index), value);

      /* begin pop:thenPush: */
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          newValue);
      stackPointer = sp;
      return;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }
  if (fmt >= (firstShortFormat())) {
    if ((((usqInt)value)) > 0xFFFF) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      return;
    }
    numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 1;
    if ((((usqInt)index)) < numSlots) {
      /* storeShort16:ofObject:withValue: */
      shortAtput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 1)))),
                 value);

      /* begin pop:thenPush: */
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          newValue);
      stackPointer = sp;
      return;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }
  if (fmt == (sixtyFourBitIndexableFormat())) {
    numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 3;
    if ((((usqInt)index)) < numSlots) {
      /* storeLong64:ofObject:withValue: */
      long64Atput(
          (void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 3)))),
          value);

      /* begin pop:thenPush: */
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          newValue);
      stackPointer = sp;
      return;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }
  if (fmt >= (firstLongFormat())) {
    if ((((usqInt)value)) > 0xFFFFFFFFU) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      return;
    }
    numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 2;
    if ((((usqInt)index)) < numSlots) {
      /* storeLong32:ofObject:withValue: */
      long32Atput(
          (void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 2)))),
          value);

      /* begin pop:thenPush: */
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          newValue);
      stackPointer = sp;
      return;
    }

    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }

  /* primitiveFailFor: */
  primFailCode = PrimErrBadReceiver;
  return;
}
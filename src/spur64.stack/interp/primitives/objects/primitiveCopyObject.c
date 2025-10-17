/* Extracted from interp.c:14536 (function primitiveCopyObject). */

/*	Primitive. Copy the state of the receiver from the argument.
        Fail if receiver and argument are of a different class.
        Fail if the receiver or argument are contexts (because of
   context-to-stack mapping). Fail if receiver and argument have different
   lengths (for indexable objects). Fail if the objects are not in a fit state
   to be copied (e.g. married contexts and Cogged methods) */

/* InterpreterPrimitives>>#primitiveCopyObject */

static void primitiveCopyObject(void) {
  sqInt arg;
  sqInt fmt;
  sqInt i;
  usqInt length;
  usqInt numBytes;
  usqInt numSlots;
  sqInt rcvr;
  sqInt valuePointer;

  if (!(argumentCount >= 1)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    return;
  }
  arg = longAt(stackPointer);
  rcvr = longAt(stackPointer + (1 * BytesPerWord));
  if (((rcvr & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  if (((arg & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  if (((longAt((void *)(rcvr))) & (classIndexMask())) !=
      ((longAt((void *)(arg))) & (classIndexMask()))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  if (isWordsOrBytesNonImm(rcvr)) {
    /* begin numBytesOf: */
    fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
    numBytes = numSlotsOf(rcvr);
    numBytes = (numBytes << (shiftForWord()));
    if (fmt >= (firstByteFormat())) {
      length = numBytes - (fmt & 7);
      goto l1;
    }

    /* bytes (the common case), including CompiledMethod */
    if (fmt <= (sixtyFourBitIndexableFormat())) {
      length = ((sqInt)numBytes);
      goto l1;
    }
    if (fmt >= (firstShortFormat())) {
      length = numBytes - (((fmt & 3) << 1));
      goto l1;
    }

    /* fmt >= self firstLongFormat */
    length = numBytes - (((fmt & 1) << 2));
    /* end numBytesOf: */
  l1:
    if (!((((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
            (formatMask())) ==
           ((byteAt((void *)(arg + (formatFieldByteOffset())))) &
            (formatMask()))) &&
          (length == (numBytesOf(arg))))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      return;
    }
    memcpy(((void *)(rcvr + BaseHeaderSize)), ((void *)(arg + BaseHeaderSize)),
           length);
  } else {
    if (!(isAppropriateForCopyObject(rcvr))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadReceiver;
      return;
    }

    length = numSlotsOf(rcvr);
    if (!((isAppropriateForCopyObject(arg)) && (length == (lengthOf(arg))))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      return;
    }
    for (i = 0; i < length; i += 1) {
      valuePointer = fetchPointerofObject(i, arg);

      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(i, rcvr, valuePointer));
      assert(isNonImmediate(rcvr));
      if (oopisGreaterThanOrEqualTo(rcvr, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(valuePointer & (tagMask())))) &&
            (oopisLessThan(valuePointer, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(rcvr);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((rcvr + BaseHeaderSize) +
                         ((((usqInt)(i) << (shiftForWord()))))),
                valuePointer);
    }
  }

  /* Note: The above could be faster for young receivers but I don't think it'll
   * matter */

  /* begin pop: */
  stackPointer += argumentCount * BytesPerWord;
}
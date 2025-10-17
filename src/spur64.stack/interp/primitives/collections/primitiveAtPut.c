/* Extracted from interp.c:12237 (function primitiveAtPut). */

/* InterpreterPrimitives>>#primitiveAtPut */

static void primitiveAtPut(void) {
  sqInt class;
  sqInt fieldIndex;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  sqInt index;
  usqInt numSlots;
  sqInt rcvr;
  sqInt signedValueToStore;
  char *sp;
  sqInt spSqInt;
  usqInt spUsqInt;
  sqInt stSize;
  sqInt totalLength;
  usqLong unsigned64BitValueToStore;
  unsigned int unsignedValueToStore;
  sqInt value;

  /* begin commonAtPut: */
  /* begin initPrimCall */
  primFailCode = 0;
  rcvr = longAt(stackPointer + (2 * BytesPerWord));
  index = longAt(stackPointer + (1 * BytesPerWord));
  value = longAt(stackPointer);
  if (((rcvr & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrInappropriate;
    goto l11;
  }

  /* No need to test for large positive integers here.  No object has 1g
   * elements */
  if (((!(index & (smallIntegerTag())))) ||
      ((argumentCount > 2) &&
       ((!((longAt((void *)(rcvr))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    goto l11;
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
    goto l11;
  }
  index = (index >> 3);

  /* begin stObject:at:put: */
  hdr = long64At((void *)(rcvr));
  fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

  /* begin lengthOf:baseHeader:format: */
  /* begin lengthOf:format: */
  numSlots = numSlotsOfAny(rcvr);
  if (fmt <= (ephemeronFormat())) {
    totalLength = numSlots;
    goto l7;
  }
  if (fmt >= (firstByteFormat())) {
    totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
    goto l7;
  }

  /* bytes, including CompiledMethod */
  if (fmt >= (firstShortFormat())) {
    totalLength = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
    goto l7;
  }
  if (fmt >= (firstLongFormat())) {
    totalLength = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
    goto l7;
  }
  if (fmt == (sixtyFourBitIndexableFormat())) {
    totalLength = numSlots;
    goto l7;
  }

  /* fmt = self forwardedFormat */
  totalLength = 0;
  /* end lengthOf:baseHeader:format: */
l7:

  /* begin fixedFieldsOf:format:length: */
  if ((fmt >= (sixtyFourBitIndexableFormat())) || (fmt == (arrayFormat()))) {
    fixedFields = 0;
    goto l9;
  }
  if (fmt < (arrayFormat())) {
    fixedFields = totalLength;
    goto l9;
  }
  class = fetchClassOfNonImm(rcvr);
  fixedFields =
      (((fetchPointerofObject(InstanceSpecificationIndex, class)) >> 3)) &
      ((1U << (fixedFieldsFieldWidth())) - 1);
  /* end fixedFieldsOf:format:length: */
l9:
  if ((fmt == (indexablePointersFormat())) &&
      ((hdr & (classIndexMask())) == ClassMethodContextCompactIndex)) {
    /* begin stackPointerForMaybeMarriedContext: */
    if (/* isStillMarriedContext: */
        (((((fetchPointerofObject(SenderIndex, rcvr))) & 7) == 1)) &&
        (!(isWidowedContext(rcvr)))) {
      spUsqInt = stackPointerIndexForFrame(frameOfMarriedContext(rcvr));
      assert((ReceiverIndex + ((spUsqInt >> 3))) < (lengthOf(rcvr)));
      stSize = spUsqInt;
      goto l6;
    }

    /* begin fetchStackPointerOf: */
    spSqInt = fetchPointerofObject(StackPointerIndex, rcvr);
    if (!((((spSqInt) & 7) == 1))) {
      stSize = 0;
      goto l6;
    }
    assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(rcvr)));
    stSize = (spSqInt >> 3);
    /* end stackPointerForMaybeMarriedContext: */
  l6:
    if ((oopisGreaterThanOrEqualTo(index, 1)) &&
        ((oopisLessThanOrEqualTo(index, stSize)) &&
         (/* isStillMarriedContext: */
          (((((fetchPointerofObject(SenderIndex, rcvr))) & 7) == 1)) &&
          (!(isWidowedContext(rcvr)))))) {
      temporaryinput(index - 1, frameOfMarriedContext(rcvr), value);
      goto l10;
    }
  } else {
    stSize = totalLength - fixedFields;
  }
  if ((oopisGreaterThanOrEqualTo(
          index, /* firstValidIndexOfIndexableObject:withFormat: */
          (fmt >= (firstCompiledMethodFormat())
               ? (((literalCountOf(rcvr)) + LiteralStart) * BytesPerOop) + 1
               : 1))) &&
      (oopisLessThanOrEqualTo(index, stSize))) {
    /* begin subscript:with:storing:format: */
    if (fmt <= 5 /* lastPointerFormat */) {
      fieldIndex = (index + fixedFields) - 1;

      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(fieldIndex, rcvr, value));
      assert(isNonImmediate(rcvr));
      if (oopisGreaterThanOrEqualTo(rcvr, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(value & (tagMask())))) &&
            (oopisLessThan(value, oldSpaceStart))) {
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
                value);
      goto l8;
    }
    if (fmt >= (firstByteFormat())) {
      if (!((((value) & 7) == 1))) {
        primFailCode = PrimErrBadArgument;
        goto l8;
      }
      signedValueToStore = (value >> 3);
      if (!((signedValueToStore >= 0) && (signedValueToStore <= 0xFF))) {
        primFailCode = PrimErrBadArgument;
        goto l8;
      }

      /* storeByte:ofObject:withValue: */
      byteAtput((void *)((rcvr + BaseHeaderSize) + ((index + fixedFields) - 1)),
                signedValueToStore);
      goto l8;
    }
    if (fmt >= (firstShortFormat())) {
      if (!((((value) & 7) == 1))) {
        primFailCode = PrimErrBadArgument;
        goto l8;
      }
      signedValueToStore = (value >> 3);
      if (!((signedValueToStore >= 0) && (signedValueToStore <= 0xFFFF))) {
        primFailCode = PrimErrBadArgument;
        goto l8;
      }

      /* storeShort16:ofObject:withValue: */
      shortAtput((void *)((rcvr + BaseHeaderSize) +
                          ((((usqInt)(((index + fixedFields) - 1)) << 1)))),
                 signedValueToStore);
      goto l8;
    }
    if (fmt == (sixtyFourBitIndexableFormat())) {
      unsigned64BitValueToStore = positive64BitValueOf(value);
      if (!primFailCode) {
        /* storeLong64:ofObject:withValue: */
        long64Atput((void *)((rcvr + BaseHeaderSize) +
                             ((((usqInt)(((index + fixedFields) - 1)) << 3)))),
                    unsigned64BitValueToStore);
      }
      goto l8;
    }

    /* 32bit-word type objects */
    unsignedValueToStore = positive32BitValueOf(value);
    if (!primFailCode) {
      /* storeLong32:ofObject:withValue: */
      long32Atput((void *)((rcvr + BaseHeaderSize) +
                           ((((usqInt)(((index + fixedFields) - 1)) << 2)))),
                  unsignedValueToStore);
    }
    /* end subscript:with:storing:format: */
  l8:;
  } else {
    /* primitiveFailFor: */
    primFailCode = (fmt <= 1 ? PrimErrBadReceiver : PrimErrBadIndex);
  }
  /* end stObject:at:put: */
l10:
  if (!primFailCode) {
    /* begin pop:thenPush: */
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              value);
    stackPointer = sp;
  }
  /* end commonAtPut: */
l11:;
}
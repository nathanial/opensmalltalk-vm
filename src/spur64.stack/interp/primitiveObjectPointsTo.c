/* Extracted from interp.c:69105 (function primitiveObjectPointsTo). */

/* LRPCheck */

/*	This primitive is assumed to be fast (see e.g.
        MethodDictionary>>includesKey:) so make it so.
        N.B. Works correctly for cogged methods too. */

/* StackInterpreterPrimitives>>#primitiveObjectPointsTo */

static void primitiveObjectPointsTo(void) {
  usqLong fmt;
  sqLong header;
  sqInt i;
  sqInt methodHeader;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt rcvr;
  sqInt thang;
  sqInt trueOrFalse;

  thang = longAt(stackPointer);
  rcvr = longAt(stackPointer + (1 * BytesPerWord));
  if (((rcvr & (tagMask())) != 0)) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, falseObj);
    return;
  }

  /* Inlined version of lastPointerOf: for speed in determining if rcvr is a
   * context. */
  header = long64At((void *)(rcvr));
  fmt = (((usqLong)(header)) >> (formatShift())) & (formatMask());
  if (fmt <= 5 /* lastPointerFormat */) {
    if ((fmt == (indexablePointersFormat())) &&
        ((header & (classIndexMask())) == ClassMethodContextCompactIndex)) {
      if (((((longAt(
                (void *)((rcvr + BaseHeaderSize) +
                         ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
            7) == 1)) {
        /* begin externalWriteBackHeadFramePointers */
        assert((framePointer - stackPointer) <
               (LargeContextSlots * BytesPerOop));
        assert(stackPage == (mostRecentlyUsedPage));
        assert(!((isFree(stackPage))));

        /* begin setHeadFP:andSP:inPage: */
        assert(stackPointer < framePointer);
        assert((stackPointer < ((stackPage->baseAddress))) &&
               (stackPointer > (((stackPage->realStackLimit)) -
                                (LargeContextSlots * BytesPerOop))));
        assert((framePointer < ((stackPage->baseAddress))) &&
               (framePointer > (((stackPage->realStackLimit)) -
                                ((LargeContextSlots * BytesPerOop) / 2))));
        (stackPage->headFP = framePointer);
        (stackPage->headSP = stackPointer);
        assert(pageListIsWellFormed());
        if (/* isStillMarriedContext: */
            (((((longAt((
                   void *)((rcvr + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
               7) == 1)) &&
            (!(isWidowedContext(rcvr)))) {
          trueOrFalse =
              marriedContextpointsTostackDeltaForCurrentFrame(rcvr, thang, 2);

          /* begin pop:thenPushBool: */
          popthenPushBool(2, /* booleanObjectOf: */
                          (trueOrFalse ? trueObj : falseObj));
          return;
        }
      }

      /* contexts end at the stack pointer */
      numSlots = CtxtTempFrameStart + (fetchStackPointerOf(rcvr));
    } else {
      /* begin numSlotsOf: */
      assert((classIndexOf(rcvr)) > (isForwardedObjectClassIndexPun()));
      numSlots =
          (((numSlotsUsqInt =
                 byteAt((void *)(rcvr + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)((
                     (sqInt)((usqInt)((longAt((void *)(rcvr - BaseHeaderSize))))
                             << 8)))))) >>
                     8
               : numSlotsUsqInt);
    }
  } else {
    if (fmt < (firstCompiledMethodFormat())) {
      /* begin pop:thenPushBool: */
      popthenPushBool(2, falseObj);
      return;
    }

    /* no pointers
       CompiledMethod: contains both pointers and bytes: */

    /* begin methodHeaderOf: */
    assert(isCompiledMethod(rcvr));
    methodHeader =
        longAt((void *)((rcvr + BaseHeaderSize) +
                        ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
    if (methodHeader == thang) {
      /* begin pop:thenPushBool: */
      popthenPushBool(2, trueObj);
      return;
    }
    numSlots = ((/* begin literalCountOfMethodHeader: */
                 assert((((methodHeader) & 7) == 1)),
                 /* literalCountOfAlternateHeader: */
                 ((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) +
               LiteralStart;
  }
  assert((((numSlots - 1) * BytesPerOop) + BaseHeaderSize) ==
         (lastPointerOf(rcvr)));
  for (i = BaseHeaderSize;
       i <= (((numSlots - 1) * BytesPerOop) + BaseHeaderSize);
       i += BytesPerOop) {
    if ((longAt((void *)(rcvr + i))) == thang) {
      /* begin pop:thenPushBool: */
      popthenPushBool(2, trueObj);
      return;
    }
  }

  /* begin pop:thenPushBool: */
  popthenPushBool(2, falseObj);
}
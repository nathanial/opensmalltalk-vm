/* Extracted from interp.c:21521 (function primitiveScanCharacters). */

/*	The character scanner primitive
        primScanCharactersFrom: startIndex to: stopIndex in: sourceString
   rightX: rightX stopConditions: stops kern: kernDelta This is the inner loop
   of measurement and scanning for text display. March through sourceString from
   startIndex to stopIndex. If any character is flagged with a non-nil entry in
   stops, then return the corresponding value. Determine width of each character
        from xTable, indexed by map. If destX would exceed rightX, then return
        stops at: 258. Advance destX by the width of the
        character. If stopIndex has been reached, then return stops at: 257.

        Receiver inst vars:
        destX			horizontal position for next character (distance
   from left of composition area) lastIndex		the Integer index of
   next character to be processed in the sourceString argument xTable
   an array mapping character code to glyph x coordinate in a form of glyphs map
   an array mapping character code to glyph position.

        Failure codes:
        PrimErrBadArgument	one of the indices is not a SmallInteger, or the
   sting argument is not a byte string, or the stops array is too small.
        PrimErrBadReceiver	the receiver has less than four inst vars or any
   of scanDestX & scanLastIndex are not SmallIntegers, or the scanXTable or
        scanMap are not arrays of the right size
        PrimErrBadIndex		either a glyph index in scanMap or a start index
   in scanXTable are not SmallIntegers PrimErrLimitExceeded	arithmetic on
   scanDestX has wrapped around to become negative
 */

/* InterpreterPrimitives>>#primitiveScanCharacters */

static void primitiveScanCharacters(void) {
  sqInt ascii;
  sqInt glyphIndex;
  sqInt kernDelta;
  sqInt maxGlyph;
  sqInt nextDestX;
  sqInt rcvr;
  sqInt scanDestX;
  sqInt scanLastIndex;
  sqInt scanMap;
  sqInt scanRightX;
  sqInt scanStartIndex;
  sqInt scanStopIndex;
  sqInt scanXTable;
  sqInt sourceString;
  sqInt sourceX;
  sqInt sourceX2;
  char *sp;
  char *sp1;
  sqInt stopReason;
  sqInt stops;

  if (argumentCount != 6) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    return;
  }

  /* Load the receiver and arguments */
  kernDelta = longAt(stackPointer);
  stops = longAt(stackPointer + (1 * BytesPerWord));
  scanRightX = longAt(stackPointer + (2 * BytesPerWord));
  sourceString = longAt(stackPointer + (3 * BytesPerWord));
  scanStopIndex = longAt(stackPointer + (4 * BytesPerWord));
  scanStartIndex = longAt(stackPointer + (5 * BytesPerWord));
  rcvr = longAt(stackPointer + (6 * BytesPerWord));
  if (!(((((kernDelta) & 7) == 1)) &&
        (((((scanRightX) & 7) == 1)) && (((((scanStopIndex) & 7) == 1)) &&
                                         ((((scanStartIndex) & 7) == 1)))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  kernDelta = (kernDelta >> 3);
  scanRightX = (scanRightX >> 3);
  scanStopIndex = (scanStopIndex >> 3);
  scanStartIndex = (scanStartIndex >> 3);

  /* check argument type and range and rcvr */
  if (!((/* isArray: */
         ((!(stops & (tagMask())))) &&
         (((byteAt((void *)(stops + (formatFieldByteOffset())))) &
           (formatMask())) == (arrayFormat()))) &&
        (((slotSizeOf(stops)) >= 258) &&
         ((/* isBytes: */
           ((!(sourceString & (tagMask())))) &&
           (((byteAt((void *)(sourceString + (formatFieldByteOffset())))) &
             (formatMask())) >= (firstByteFormat()))) &&
          ((scanStartIndex > 0) &&
           ((scanStopIndex > 0) &&
            (scanStopIndex <= (byteSizeOf(sourceString))))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  if (!((/* isPointers: */
         ((!(rcvr & (tagMask())))) &&
         (((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */)) &&
        ((slotSizeOf(rcvr)) >= 4))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }

  /* Check required rcvr instVars */
  scanDestX =
      longAt((void *)((rcvr + BaseHeaderSize) + (0U << (shiftForWord()))));
  scanLastIndex =
      longAt((void *)((rcvr + BaseHeaderSize) + (1U << (shiftForWord()))));
  scanXTable =
      longAt((void *)((rcvr + BaseHeaderSize) + (2U << (shiftForWord()))));
  scanMap =
      longAt((void *)((rcvr + BaseHeaderSize) + (3U << (shiftForWord()))));
  if (!((/* isArray: */
         ((!(scanXTable & (tagMask())))) &&
         (((byteAt((void *)(scanXTable + (formatFieldByteOffset())))) &
           (formatMask())) == (arrayFormat()))) &&
        ((/* isArray: */
          ((!(scanMap & (tagMask())))) &&
          (((byteAt((void *)(scanMap + (formatFieldByteOffset())))) &
            (formatMask())) == (arrayFormat()))) &&
         (((slotSizeOf(scanMap)) == 0x100) &&
          (((((scanDestX) & 7) == 1)) && ((((scanLastIndex) & 7) == 1))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  if (((scanDestX = (scanDestX >> 3))) < 0) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  scanLastIndex = (scanLastIndex >> 3);
  maxGlyph = (slotSizeOf(scanXTable)) - 2;

  /* Okay, here we go. We have eliminated nearly all failure
     conditions, to optimize the inner fetches. */
  scanLastIndex = scanStartIndex;
  while (scanLastIndex <= scanStopIndex) {
    ascii =
        byteAt((void *)((sourceString + BaseHeaderSize) + (scanLastIndex - 1)));

    /* Known to be okay since stops size >= 258 */
    if (!(((stopReason =
                longAt((void *)((stops + BaseHeaderSize) +
                                ((((usqInt)(ascii) << (shiftForWord())))))))) ==
          nilObj)) {
      if (!(scanDestX >= 0)) {
        /* primitiveFailFor: */
        primFailCode = PrimErrLimitExceeded;
        return;
      }

      /* begin storeInteger:ofObject:withValue: */
      if ((((((usqInt)(scanDestX)) >> 60) + 1) & 15) <= 1) {
        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(rcvr)) && (!(isForwarded(rcvr))));
        assert(validStorePointerUncheckedArgs(0, rcvr,
                                              (((usqInt)scanDestX << 3) | 1)));
        longAtput((void *)((rcvr + BaseHeaderSize) + (0U << (shiftForWord()))),
                  (((usqInt)scanDestX << 3) | 1));
      } else {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
      }

      /* begin storeInteger:ofObject:withValue: */
      if ((((((usqInt)(scanLastIndex)) >> 60) + 1) & 15) <= 1) {
        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(rcvr)) && (!(isForwarded(rcvr))));
        assert(validStorePointerUncheckedArgs(
            1, rcvr, (((usqInt)scanLastIndex << 3) | 1)));
        longAtput((void *)((rcvr + BaseHeaderSize) + (1U << (shiftForWord()))),
                  (((usqInt)scanLastIndex << 3) | 1));
      } else {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
      }

      /* begin methodReturnValue: */
      assert(!((failed())));
      longAtput(
          (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          stopReason);
      stackPointer = sp;
      return;
    }

    /* Store everything back and get out of here since some stop condition needs
       to be checked Known to be okay since scanMap size = 256 */
    glyphIndex = longAt((void *)((scanMap + BaseHeaderSize) +
                                 ((((usqInt)(ascii) << (shiftForWord()))))));

    /* fail if the glyphIndex is out of range */
    if (!(((((glyphIndex) & 7) == 1)) &&
          ((((glyphIndex = (glyphIndex >> 3))) >= 0) &&
           (glyphIndex <= maxGlyph)))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadIndex;
      return;
    }
    sourceX = longAt((void *)((scanXTable + BaseHeaderSize) +
                              ((((usqInt)(glyphIndex) << (shiftForWord()))))));
    sourceX2 =
        longAt((void *)((scanXTable + BaseHeaderSize) +
                        ((((usqInt)((glyphIndex + 1)) << (shiftForWord()))))));

    /* Above may fail if non-integer entries in scanXTable */
    if (!(((((sourceX) & 7) == 1)) && ((((sourceX2) & 7) == 1)))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadIndex;
      return;
    }
    nextDestX = (scanDestX + ((sourceX2 >> 3))) - ((sourceX >> 3));
    if (nextDestX > scanRightX) {
      /* begin storeInteger:ofObject:withValue: */
      if ((((((usqInt)(scanDestX)) >> 60) + 1) & 15) <= 1) {
        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(rcvr)) && (!(isForwarded(rcvr))));
        assert(validStorePointerUncheckedArgs(0, rcvr,
                                              (((usqInt)scanDestX << 3) | 1)));
        longAtput((void *)((rcvr + BaseHeaderSize) + (0U << (shiftForWord()))),
                  (((usqInt)scanDestX << 3) | 1));
      } else {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
      }

      /* begin storeInteger:ofObject:withValue: */
      if ((((((usqInt)(scanLastIndex)) >> 60) + 1) & 15) <= 1) {
        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(rcvr)) && (!(isForwarded(rcvr))));
        assert(validStorePointerUncheckedArgs(
            1, rcvr, (((usqInt)scanLastIndex << 3) | 1)));
        longAtput((void *)((rcvr + BaseHeaderSize) + (1U << (shiftForWord()))),
                  (((usqInt)scanLastIndex << 3) | 1));
      } else {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
      }

      /* begin methodReturnValue: */
      assert(!((failed())));
      longAtput(
          (sp1 = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
          longAt((void *)((stops + BaseHeaderSize) +
                          ((((usqInt)((CrossedX - 1)) << (shiftForWord())))))));
      stackPointer = sp1;
      return;
    }

    /* Store everything back and get out of here since we got to the right edge
     */
    if (!((nextDestX >= 0) && (((scanDestX = nextDestX + kernDelta)) >= 0))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrLimitExceeded;
      return;
    }
    scanLastIndex += 1;
  }

  /* Known to be okay since scanStartIndex > 0 and scanStopIndex <= sourceString
   * size */

  /* begin storeInteger:ofObject:withValue: */
  if ((((((usqInt)(scanDestX)) >> 60) + 1) & 15) <= 1) {
    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(rcvr)) && (!(isForwarded(rcvr))));
    assert(validStorePointerUncheckedArgs(0, rcvr,
                                          (((usqInt)scanDestX << 3) | 1)));
    longAtput((void *)((rcvr + BaseHeaderSize) + (0U << (shiftForWord()))),
              (((usqInt)scanDestX << 3) | 1));
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }

  /* begin storeInteger:ofObject:withValue: */
  if ((((((usqInt)(scanStopIndex)) >> 60) + 1) & 15) <= 1) {
    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(rcvr)) && (!(isForwarded(rcvr))));
    assert(validStorePointerUncheckedArgs(1, rcvr,
                                          (((usqInt)scanStopIndex << 3) | 1)));
    longAtput((void *)((rcvr + BaseHeaderSize) + (1U << (shiftForWord()))),
              (((usqInt)scanStopIndex << 3) | 1));
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }

  /* begin methodReturnValue: */
  assert(!((failed())));
  longAtput(
      (sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
      longAt((void *)((stops + BaseHeaderSize) +
                      ((((usqInt)((EndOfRun - 1)) << (shiftForWord())))))));
  stackPointer = sp;
}
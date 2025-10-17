/* Extracted from interp.c:16484 (function primitiveFormPrint). */

/*	On platforms that support it, this primitive prints the receiver,
   assumed to be a Form, to the default printer.
 */

/* InterpreterPrimitives>>#primitiveFormPrint */

static void primitiveFormPrint(void) {
  sqInt bitsArray;
  sqInt bitsArraySize;
  sqInt depth;
  sqInt fmt;
  sqInt h;
  double hScale;
  sqInt landscapeFlag;
  usqInt numBytes;
  usqInt numSlots;
  sqInt pixelsPerWord;
  sqInt rcvr;
  sqInt successBoolean;
  double vScale;
  sqInt w;
  sqInt wordsPerLine;

  bitsArray = 0;
  depth = 0;
  h = 0;
  w = 0;

  /* begin booleanValueOf: */
  if ((longAt(stackPointer)) == trueObj) {
    landscapeFlag = 1;
    goto l1;
  }
  if ((longAt(stackPointer)) == falseObj) {
    landscapeFlag = 0;
    goto l1;
  }

  /* begin success: */
  if (!primFailCode) {
    primFailCode = 1;
  }
  landscapeFlag = null;
  /* end booleanValueOf: */
l1:
  vScale = floatValueOf(longAt(stackPointer + (1 * BytesPerWord)));
  hScale = floatValueOf(longAt(stackPointer + (2 * BytesPerWord)));
  rcvr = longAt(stackPointer + (3 * BytesPerWord));
  if (!((/* isPointers: */
         ((!(rcvr & (tagMask())))) &&
         (((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */)) &&
        ((lengthOf(rcvr)) >= 4))) {
    /* begin success: */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
  if (!primFailCode) {
    bitsArray = fetchPointerofObject(0U, rcvr);
    w = fetchIntegerofObject(1, rcvr);
    h = fetchIntegerofObject(2, rcvr);
    depth = fetchIntegerofObject(3, rcvr);
    if (!((w > 0) && (h > 0))) {
      /* begin success: */
      if (!primFailCode) {
        primFailCode = 1;
      }
    }
    pixelsPerWord = 32 / depth;
    wordsPerLine = (w + (pixelsPerWord - 1)) / pixelsPerWord;
    if (/* isWordsOrBytes: */
        ((!(bitsArray & (tagMask())))) && (isWordsOrBytesNonImm(bitsArray))) {
      /* begin numBytesOf: */
      fmt = (byteAt((void *)(bitsArray + (formatFieldByteOffset())))) &
            (formatMask());
      numBytes = numSlotsOf(bitsArray);
      numBytes = (numBytes << (shiftForWord()));
      if (fmt >= (firstByteFormat())) {
        bitsArraySize = numBytes - (fmt & 7);
        goto l2;
      }

      /* bytes (the common case), including CompiledMethod */
      if (fmt <= (sixtyFourBitIndexableFormat())) {
        bitsArraySize = numBytes;
        goto l2;
      }
      if (fmt >= (firstShortFormat())) {
        bitsArraySize = numBytes - (((fmt & 3) << 1));
        goto l2;
      }

      /* fmt >= self firstLongFormat */
      bitsArraySize = numBytes - (((fmt & 1) << 2));
      /* end numBytesOf: */
    l2:

      /* begin success: */
      if (!(bitsArraySize == ((wordsPerLine * h) * 4))) {
        if (!primFailCode) {
          primFailCode = 1;
        }
      }
    } else {
      /* begin success: */
      if (!primFailCode) {
        primFailCode = 1;
      }
    }
  }
  if (!primFailCode) {
    successBoolean = ioFormPrint(bitsArray + BaseHeaderSize, w, h, depth,
                                 hScale, vScale, landscapeFlag);

    /* begin success: */
    if (!successBoolean) {
      if (!primFailCode) {
        primFailCode = 1;
      }
    }
    if (!primFailCode) {
      /* begin methodReturnReceiver */
      assert(!((failed())));
      stackPointer += argumentCount * BytesPerWord;
    }
  }
}
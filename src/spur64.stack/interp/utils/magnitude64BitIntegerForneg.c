/* Extracted from interp.c:11274 (function magnitude64BitIntegerForneg). */

/*	Return a Large Integer object for the given integer magnitude and sign
 */

/* InterpreterPrimitives>>#magnitude64BitIntegerFor:neg: */

static sqInt magnitude64BitIntegerForneg(usqLong magnitude, sqInt isNegative) {
  int isSmall;
  int largeClassIndex;
  usqInt newLargeInteger;
  usqInt newObj;
  usqInt numBytesUsqInt;
  sqInt numSlots;
  sqInt objFormat;
  sqInt smallVal;
  sqInt sz;

  isSmall = (isNegative ? magnitude <= ((MaxSmallInteger) + 1)
                        : magnitude <= (MaxSmallInteger));
  if (isSmall) {
    smallVal = ((sqInt)magnitude);
    if (isNegative) {
      smallVal = 0 - smallVal;
    }
    return (((usqInt)smallVal << 3) | 1);
  }
  largeClassIndex = (isNegative ? ClassLargeNegativeIntegerCompactIndex
                                : ClassLargePositiveIntegerCompactIndex);
  sz = 8;
  objFormat = (firstByteFormat()) + ((8 - sz) & (BytesPerWord - 1));

  /* begin eeInstantiateSmallClassIndex:format:numBytes: */
  assert((sz >= 0) && ((largeClassIndex != 0) &&
                       ((knownClassAtIndex(largeClassIndex)) != nilObj)));
  assert(((objFormat < (firstByteFormat()) ? objFormat
                                           : objFormat & (byteFormatMask()))) ==
         (instSpecOfClass(knownClassAtIndex(largeClassIndex))));
  numSlots = ((sz + BytesPerWord) - 1) / BytesPerWord;

  /* begin allocateSmallNewSpaceSlots:format:classIndex: */
  assert(numSlots < (numSlotsMask()));
  newObj = freeStart;
  numBytesUsqInt = BaseHeaderSize + ((numSlots < 1 ? 8 /* allocationUnit */
                                                   : numSlots * BytesPerOop));
  assert((numBytesUsqInt % (allocationUnit())) == 0);
  assert((newObj % (allocationUnit())) == 0);
  if ((freeStart + numBytesUsqInt) > scavengeThreshold) {
    if (!needGCFlag) {
      /* begin scheduleScavenge */
      needGCFlag = 1;
      forceInterruptCheck();
    }
    if ((freeStart + numBytesUsqInt) > (((eden).limit))) {
      error(
          "no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
      newLargeInteger = 0;
      goto l1;
    }
  }
  long64Atput((void *)(newObj),
              ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
               ((((usqInt)(objFormat) << (formatShift()))))) +
                  largeClassIndex);
  freeStart += numBytesUsqInt;
  newLargeInteger = newObj;
  /* end eeInstantiateSmallClassIndex:format:numBytes: */
l1:

  /* storeLong64:ofObject:withValue: */
  long64Atput((void *)((newLargeInteger + BaseHeaderSize)),
              SQ_SWAP_8_BYTES_IF_BIGENDIAN(magnitude));

  /* Memory is eight byte aligned in SPUR, so we are sure to have room for
   * 64bits word whatever allocated sz */
  return newLargeInteger;
}
/* Extracted from interp.c:66182 (function linkExternalCallerrInto). */

/*	The function has not been loaded yet. Fetch module and function name. */

/* StackInterpreterPrimitives>>#linkExternalCall:errInto: */

static void (*linkExternalCallerrInto(sqInt externalCallLiteral,
                                      sqInt *failPtr))(void) {
  void (*addr)();
  sqInt fmt;
  void *functionAddress;
  sqInt functionLength;
  sqInt functionName;
  sqInt i;
  sqInt index;
  sqInt metadata;
  sqInt moduleLength;
  sqInt moduleName;
  usqInt numSlots;

  metadata = 0;
  moduleName = fetchPointerofObject(ExternalCallLiteralModuleNameIndex, externalCallLiteral);
  if (moduleName == nilObj) {
    moduleLength = 0;
  } else {
    if (!(/* isBytes: */
          ((!(moduleName & (tagMask())))) &&
          (((byteAt((void *)(moduleName + (formatFieldByteOffset())))) &
            (formatMask())) >= (firstByteFormat())))) {
      if (failPtr) {
        failPtr[0] = PrimErrBadMethod;
      }
      return 0;
    }

    /* begin numBytesOfBytes: */
    fmt = (byteAt((void *)(moduleName + (formatFieldByteOffset())))) &
          (formatMask());
    assert(fmt >= (firstByteFormat()));
    moduleLength =
        ((((/* begin numSlotsOf: */
            assert((classIndexOf(moduleName)) >
                   (isForwardedObjectClassIndexPun())),
            (((numSlots = byteAt(
                   (void *)(moduleName + (numSlotsFieldByteOffset()))))) ==
                     (numSlotsMask())
                 ? ((((usqInt)((
                       (sqInt)((usqInt)((longAt(
                                   (void *)(moduleName - BaseHeaderSize))))
                               << 8)))))) >>
                       8
                 : numSlots)))
          << (shiftForWord()))) -
        (fmt & 7);
  }
  functionName =
      fetchPointerofObject(ExternalCallLiteralFunctionNameIndex, externalCallLiteral);
  if (!(/* isBytes: */
        ((!(functionName & (tagMask())))) &&
        (((byteAt((void *)(functionName + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstByteFormat())))) {
    if (failPtr) {
      failPtr[0] = PrimErrBadMethod;
    }
    return 0;
  }

  /* begin numBytesOfBytes: */
  fmt = (byteAt((void *)(functionName + (formatFieldByteOffset())))) &
        (formatMask());
  assert(fmt >= (firstByteFormat()));
  functionLength =
      ((((/* begin numSlotsOf: */
          assert((classIndexOf(functionName)) >
                 (isForwardedObjectClassIndexPun())),
          (((numSlots = byteAt(
                 (void *)(functionName + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)((
                     (sqInt)((usqInt)((longAt(
                                 (void *)(functionName - BaseHeaderSize))))
                             << 8)))))) >>
                     8
               : numSlots)))
        << (shiftForWord()))) -
      (fmt & 7);

  /* Spur needs metadata for the primitive, which is stored in the third slot of
   * the literal. */
  addr = ioLoadExternalFunctionOfLengthFromModuleOfLengthMetadataInto(
      functionName + BaseHeaderSize, functionLength,
      moduleName + BaseHeaderSize, moduleLength, (&metadata));
  if (addr) {
    assert(((((metadata) >> SpurPrimitiveAccessorDepthShift) >= -1) &&
            (((metadata) >> SpurPrimitiveAccessorDepthShift) <= 5)));
    functionAddress = ((void *)addr);

    /* begin addToExternalPrimitiveTable: */
    for (i = externalPrimitiveTableFirstFreeIndex;
         i < MaxExternalPrimitiveTableSize; i += 1) {
      if (!(externalPrimitiveTable[i])) {
        externalPrimitiveTable[i] = functionAddress;
        index = (externalPrimitiveTableFirstFreeIndex = i + 1);
        goto l1;
      }
    }

    /* if no space left, return zero so it'll looked up again */
    index = 0;
    /* end addToExternalPrimitiveTable: */
  l1:

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(externalCallLiteral)) &&
           (!(isForwarded(externalCallLiteral))));
    assert(validStorePointerUncheckedArgs(ExternalCallLiteralFlagsIndex,
                                          externalCallLiteral,
                                          (((usqInt)metadata << 3) | 1)));
    longAtput((void *)((externalCallLiteral + BaseHeaderSize) +
                       ((((usqInt)(ExternalCallLiteralFlagsIndex)
                          << (shiftForWord()))))),
              (((usqInt)metadata << 3) | 1));
  } else {
    index = -1;
  }

  /* Store the index (or -1 if failure) back in the literal */

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(externalCallLiteral)) &&
         (!(isForwarded(externalCallLiteral))));
  assert(validStorePointerUncheckedArgs(ExternalCallLiteralTargetFunctionIndex,
                                        externalCallLiteral,
                                        (((usqInt)index << 3) | 1)));
  longAtput((void *)((externalCallLiteral + BaseHeaderSize) +
                     ((((usqInt)(ExternalCallLiteralTargetFunctionIndex)
                        << (shiftForWord()))))),
            (((usqInt)index << 3) | 1));

  /* If the function has been successfully loaded cache it */
  rewriteMethodCacheEntryForExternalPrimitiveToFunction(
      (index >= 0 ? addr : 0));
  return addr;
}
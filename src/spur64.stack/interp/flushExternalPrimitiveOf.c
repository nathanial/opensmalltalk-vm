/* Extracted from interp.c:53621 (function flushExternalPrimitiveOf). */

/*	methodObj is a CompiledMethod. If it contains an external primitive,
        flush the function address and session ID of the CM. Answer the prim
        index for the benefit of subclass overrides. */

/* StackInterpreter>>#flushExternalPrimitiveOf: */

static sqInt flushExternalPrimitiveOf(sqInt methodObj) {
  usqInt firstBytecode;
  sqInt header;
  sqInt lit;
  usqInt numSlots;
  sqInt primIdx;

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(methodObj));
  header = longAt((void *)((methodObj + BaseHeaderSize) +
                           ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

  /* primitiveIndexOfMethod:header: */
  if (((header & AlternateHeaderHasPrimFlag) != 0)) {
    firstBytecode =
        (methodObj +
         ((LiteralStart + (((header >> 3)) & AlternateHeaderNumLiteralsMask)) *
          BytesPerOop)) +
        BaseHeaderSize;
    primIdx = (byteAt((void *)(firstBytecode + 1))) +
              ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
  } else {
    primIdx = 0;
  }
  if ((primIdx == PrimNumberExternalCall) &&
      (((/* begin literalCountOfMethodHeader: */
         assert((((header) & 7) == 1)),
         /* literalCountOfAlternateHeader: */
         ((header >> 3)) & AlternateHeaderNumLiteralsMask)) > 0)) {
    lit = longAt(
        (void *)((methodObj + BaseHeaderSize) + (1U << (shiftForWord()))));
    if ((/* isArray: */
         ((!(lit & (tagMask())))) &&
         (((byteAt((void *)(lit + (formatFieldByteOffset())))) &
           (formatMask())) == (arrayFormat()))) &&
        (((/* begin numSlotsOf: */
           assert((classIndexOf(lit)) > (isForwardedObjectClassIndexPun())),
           (((numSlots =
                  byteAt((void *)(lit + (numSlotsFieldByteOffset()))))) ==
                    (numSlotsMask())
                ? ((((usqInt)((
                      (sqInt)((usqInt)((longAt((void *)(lit - BaseHeaderSize))))
                              << 8)))))) >>
                      8
                : numSlots))) == 4)) {
      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(lit)) && (!(isForwarded(lit))));
      assert(validStorePointerUncheckedArgs(2, lit, ConstZero));
      longAtput((void *)((lit + BaseHeaderSize) + (2U << (shiftForWord()))),
                ConstZero);

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(lit)) && (!(isForwarded(lit))));
      assert(validStorePointerUncheckedArgs(3, lit, ConstZero));
      longAtput((void *)((lit + BaseHeaderSize) + (3U << (shiftForWord()))),
                ConstZero);
    }
  }

  /* If not, something's broken */
  return primIdx;
}
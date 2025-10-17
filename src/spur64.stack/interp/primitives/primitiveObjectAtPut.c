/* Extracted from interp.c:19976 (function primitiveObjectAtPut). */

/*	Store a literal into a CompiledMethod at the given index. Defined for
        CompiledMethods only.
 */

/* InterpreterPrimitives>>#primitiveObjectAtPut */

static void primitiveObjectAtPut(void) {
  sqInt fieldIndex;
  sqInt index;
  sqInt newValue;
  char *sp;
  sqInt thisReceiver;

  newValue = longAt(stackPointer);
  index = longAt(stackPointer + (1 * BytesPerWord));
  if (((!(index & (smallIntegerTag())))) ||
      ((index == ConstOne) && ((!(newValue & (smallIntegerTag())))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  index = (index >> 3);
  thisReceiver = longAt(stackPointer + (2 * BytesPerWord));
  if (
#if IMMUTABILITY
      ((((usqInt)((byteAt(
            (void *)(thisReceiver + (immutableExtraBitsByteOffset())))))) >>
        (immutableBitByteShift())) &
       1) != 0
#else // IMMUTABILITY
      0
#endif
  ) {
    /* primitiveFailFor: */
    primFailCode = PrimErrNoModification;
    return;
  }
  if (!((index > 0) &&
        (index <= ((literalCountOf(thisReceiver)) + LiteralStart)))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }
  fieldIndex = index - 1;

  /* begin storePointer:ofObject:withValue: */
  assert(validStorePointerArgs(fieldIndex, thisReceiver, newValue));
  assert(isNonImmediate(thisReceiver));
  if (oopisGreaterThanOrEqualTo(thisReceiver, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(newValue & (tagMask())))) &&
        (oopisLessThan(newValue, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(thisReceiver + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(thisReceiver);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((thisReceiver + BaseHeaderSize) +
                     ((((usqInt)(fieldIndex) << (shiftForWord()))))),
            newValue);

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer + (2 * BytesPerWord)), newValue);
  stackPointer = sp;
}
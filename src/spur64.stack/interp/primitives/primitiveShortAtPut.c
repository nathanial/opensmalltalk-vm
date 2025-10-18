/* Extracted from interp.c:22429 (function primitiveShortAtPut). */

/*	Treat the receiver, which can be indexible by either bytes or words, as
   an array of signed 16-bit values. Set the contents of the given index to the
        given value.
        Note that the index specifies the i-th 16-bit entry, not the i-th byte
   or word.
 */

/* InterpreterPrimitives>>#primitiveShortAtPut */

static void primitiveShortAtPut(void) {
  sqInt index;
  sqInt rcvr;
  char *sp;
  sqInt value;

  value = longAt(stackPointer);
  index = longAt(stackPointer + (1 * BytesPerWord));
  if (!(((((value) & 7) == 1)) &&
        (((((index) & 7) == 1)) &&
         (((value = (value >> 3)), (value >= -32768) && (value <= 0x7FFF)))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  rcvr = longAt(stackPointer + (2 * BytesPerWord));
  if (!(/* isWordsOrBytes: */
        ((!(rcvr & (tagMask())))) && (isWordsOrBytesNonImm(rcvr)))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrInappropriate;
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
  index = (index >> 3);
  if (!((index >= 1) && (index <= (((usqInt)((numBytesOf(rcvr)))) >> 1)))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }

  /* storeShort16:ofObject:withValue: */
  shortAtput(
      (void *)((rcvr + BaseHeaderSize) + ((((usqInt)((index - 1)) << 1)))),
      value);

  /* begin pop:thenPush: */
  popthenPush(3, (((usqInt)value << 3) | 1));
}
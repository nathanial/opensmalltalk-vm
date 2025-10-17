/* Extracted from interp.c:22232 (function primitiveSetLogDirectory). */

/*	Primitive. Set the VM's log directory */

/* InterpreterPrimitives>>#primitiveSetLogDirectory */

EXPORT(sqInt)
primitiveSetLogDirectory(void) {
  sqInt fmt;
  usqInt numBytes;
  usqInt numSlots;
  sqInt stringOop;
  sqInt sz;

  stringOop = longAt(stackPointer);
  if (!(/* isBytes: */
        ((!(stringOop & (tagMask())))) &&
        (((byteAt((void *)(stringOop + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstByteFormat())))) {
    /* begin success: */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return null;
  }

  /* begin byteSizeOf: */
  if (((stringOop & (tagMask())) != 0)) {
    sz = 0;
    goto l1;
  }

  /* begin numBytesOf: */
  fmt = (byteAt((void *)(stringOop + (formatFieldByteOffset())))) &
        (formatMask());
  numBytes = numSlotsOf(stringOop);
  numBytes = (numBytes << (shiftForWord()));
  if (fmt >= (firstByteFormat())) {
    sz = numBytes - (fmt & 7);
    goto l1;
  }

  /* bytes (the common case), including CompiledMethod */
  if (fmt <= (sixtyFourBitIndexableFormat())) {
    sz = numBytes;
    goto l1;
  }
  if (fmt >= (firstShortFormat())) {
    sz = numBytes - (((fmt & 3) << 1));
    goto l1;
  }

  /* fmt >= self firstLongFormat */
  sz = numBytes - (((fmt & 1) << 2));
  /* end byteSizeOf: */
l1:
  ioSetLogDirectoryOfSize(firstIndexableField(stringOop), sz);
  if (!primFailCode) {
    /* begin pop: */
    stackPointer += argumentCount * BytesPerWord;
  }
  return 0;
}
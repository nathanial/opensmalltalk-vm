/* Extracted from interp.c:12479 (function primitiveBeCursor). */

/*	Set the cursor to the given shape. The Mac only supports 16x16 pixel
        cursors. Cursor offsets are handled by Smalltalk.
 */

/* InterpreterPrimitives>>#primitiveBeCursor */

static void primitiveBeCursor(void) {
  sqInt cursorBitsObj;
  sqInt cursorObj;
  sqInt depth;
  sqInt extentX;
  sqInt extentY;
  sqInt maskBitsObj;
  sqInt maskObj;
  usqInt numSlots;
  sqInt offsetObj;
  sqInt offsetX;
  sqInt offsetY;

  maskBitsObj = 0;
  if ((((usqInt)argumentCount)) > 1) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    return;
  }
  cursorObj = longAt(stackPointer + (argumentCount * BytesPerWord));
  if (!((/* isPointers: */
         ((!(cursorObj & (tagMask())))) &&
         (((byteAt((void *)(cursorObj + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */)) &&
        (((/* begin numSlotsOf: */
           assert((classIndexOf(cursorObj)) >
                  (isForwardedObjectClassIndexPun())),
           (((numSlots =
                  byteAt((void *)(cursorObj + (numSlotsFieldByteOffset()))))) ==
                    (numSlotsMask())
                ? ((((usqInt)((
                      (sqInt)((usqInt)((
                                  longAt((void *)(cursorObj - BaseHeaderSize))))
                              << 8)))))) >>
                      8
                : numSlots))) >= 5))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  cursorBitsObj =
      longAt((void *)((cursorObj + BaseHeaderSize) + (0U << (shiftForWord()))));
  offsetObj =
      longAt((void *)((cursorObj + BaseHeaderSize) + (4U << (shiftForWord()))));
  if (!(((((((extentX = longAt((void *)((cursorObj + BaseHeaderSize) +
                                        (1U << (shiftForWord()))))))) &
           7) == 1)) &&
        (((((((extentY = longAt((void *)((cursorObj + BaseHeaderSize) +
                                         (2U << (shiftForWord()))))))) &
            7) == 1)) &&
         (((((((depth = longAt((void *)((cursorObj + BaseHeaderSize) +
                                        (3U << (shiftForWord()))))))) &
             7) == 1)) &&
          ((/* isPointers: */
            ((!(offsetObj & (tagMask())))) &&
            (((byteAt((void *)(offsetObj + (formatFieldByteOffset())))) &
              (formatMask())) <= 5 /* lastPointerFormat */)) &&
           ((((/* begin numSlotsOf: */
               assert((classIndexOf(offsetObj)) >
                      (isForwardedObjectClassIndexPun())),
               (((numSlots = byteAt(
                      (void *)(offsetObj + (numSlotsFieldByteOffset()))))) ==
                        (numSlotsMask())
                    ? ((((usqInt)((
                          (sqInt)((usqInt)((longAt(
                                      (void *)(offsetObj - BaseHeaderSize))))
                                  << 8)))))) >>
                          8
                    : numSlots))) >= 2) &&
            (((((((offsetX = longAt((void *)((offsetObj + BaseHeaderSize) +
                                             (0U << (shiftForWord()))))))) &
                7) == 1)) &&
             ((((((offsetY = longAt((void *)((offsetObj + BaseHeaderSize) +
                                             (1U << (shiftForWord()))))))) &
                7) == 1))))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  offsetX = (offsetX >> 3);
  offsetY = (offsetY >> 3);
  extentX = (extentX >> 3);
  extentY = (extentY >> 3);
  depth = (depth >> 3);
  if (!argumentCount) {
    if (depth == 32) {
      if (!((extentX > 0) &&
            ((extentY > 0) &&
             ((((offsetX >= (-extentX)) && (offsetX <= 0))) &&
              ((((offsetY >= (-extentY)) && (offsetY <= 0))) &&
               ((/* isWords: */
                 ((!(cursorBitsObj & (tagMask())))) &&
                 (((((byteAt(
                         (void *)(cursorBitsObj + (formatFieldByteOffset())))) &
                     (formatMask())) >= (firstLongFormat())) &&
                   (((byteAt(
                         (void *)(cursorBitsObj + (formatFieldByteOffset())))) &
                     (formatMask())) <= ((firstShortFormat()) - 1))))) &&
                ((lengthOfformat(cursorBitsObj, firstLongFormat())) ==
                 (extentX * extentY)))))))) {
        /* primitiveFailFor: */
        primFailCode = PrimErrBadReceiver;
        return;
      }

      /* This should pass (objectMemory firstIndexableField: cursorBitsObj type:
       * #'unsigned int *'), but the platform sources are venerable and expect
       * sqInts */
      ioSetCursorARGB(((sqInt)(firstIndexableField(cursorBitsObj))), extentX,
                      extentY, offsetX, offsetY);

      /* begin methodReturnReceiver */
      assert(!((failed())));
      stackPointer += argumentCount * BytesPerWord;
      return;
    }

    /* Support arbitrary-sized 32 bit ARGB forms --bf 3/1/2007 23:51 */
    if (!((extentX == 16) &&
          ((extentY == 16) &&
           ((((offsetX >= -16) && (offsetX <= 0))) &&
            ((((offsetY >= -16) && (offsetY <= 0))) &&
             ((/* isWords: */
               ((!(cursorBitsObj & (tagMask())))) &&
               (((((byteAt(
                       (void *)(cursorBitsObj + (formatFieldByteOffset())))) &
                   (formatMask())) >= (firstLongFormat())) &&
                 (((byteAt(
                       (void *)(cursorBitsObj + (formatFieldByteOffset())))) &
                   (formatMask())) <= ((firstShortFormat()) - 1))))) &&
              ((lengthOfformat(cursorBitsObj, firstLongFormat())) == 16))))))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadReceiver;
      return;
    }

    /* This should pass (objectMemory firstIndexableField: cursorBitsObj type:
     * #'unsigned int *'), but the platform sources are venerable and expect
     * sqInts */
    ioSetCursor(((sqInt)(firstIndexableField(cursorBitsObj))), offsetX,
                offsetY);

    /* begin methodReturnReceiver */
    assert(!((failed())));
    stackPointer += argumentCount * BytesPerWord;
    return;
  }
  if (!((extentX == 16) &&
        ((extentY == 16) &&
         ((((offsetX >= -16) && (offsetX <= 0))) &&
          ((((offsetY >= -16) && (offsetY <= 0))) &&
           ((/* isWords: */
             ((!(cursorBitsObj & (tagMask())))) &&
             (((((byteAt((void *)(cursorBitsObj + (formatFieldByteOffset())))) &
                 (formatMask())) >= (firstLongFormat())) &&
               (((byteAt((void *)(cursorBitsObj + (formatFieldByteOffset())))) &
                 (formatMask())) <= ((firstShortFormat()) - 1))))) &&
            ((lengthOfformat(cursorBitsObj, firstLongFormat())) == 16))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  maskObj = longAt(stackPointer);
  if (!((/* isPointers: */
         ((!(maskObj & (tagMask())))) &&
         (((byteAt((void *)(maskObj + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */)) &&
        ((((/* begin numSlotsOf: */
            assert((classIndexOf(maskObj)) >
                   (isForwardedObjectClassIndexPun())),
            (((numSlots =
                   byteAt((void *)(maskObj + (numSlotsFieldByteOffset()))))) ==
                     (numSlotsMask())
                 ? ((((usqInt)(((sqInt)((usqInt)((longAt((
                                            void *)(maskObj - BaseHeaderSize))))
                                        << 8)))))) >>
                       8
                 : numSlots))) >= 5) &&
         (((longAt((void *)((maskObj + BaseHeaderSize) +
                            (1U << (shiftForWord()))))) ==
           ((((usqInt)16 << 3) | 1))) &&
          (((longAt((void *)((maskObj + BaseHeaderSize) +
                             (2U << (shiftForWord()))))) ==
            ((((usqInt)16 << 3) | 1))) &&
           (((longAt((void *)((maskObj + BaseHeaderSize) +
                              (3U << (shiftForWord()))))) ==
             ((((usqInt)1 << 3) | 1))) &&
            ((isWords(
                 (maskBitsObj = longAt((void *)((maskObj + BaseHeaderSize) +
                                                (0U << (shiftForWord()))))))) &&
             ((lengthOfformat(maskBitsObj, firstLongFormat())) == 16)))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }

  /* This should pass (objectMemory firstIndexableField: cursorBitsObj type:
   * #'unsigned int *'), but the platform sources are venerable and expect
   * sqInts */
  ioSetCursorWithMask(((sqInt)(firstIndexableField(cursorBitsObj))),
                      ((sqInt)(firstIndexableField(maskBitsObj))), offsetX,
                      offsetY);

  /* begin methodReturnReceiver */
  assert(!((failed())));
  stackPointer += argumentCount * BytesPerWord;
}
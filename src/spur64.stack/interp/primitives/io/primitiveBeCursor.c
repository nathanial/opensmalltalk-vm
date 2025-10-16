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
        (((assert((classIndexOf(cursorObj)) >
                  (isForwardedObjectClassIndexPun())),
           numSlotsOf(cursorObj))) >= 5))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  cursorBitsObj = fetchPointerofObject(0U, cursorObj);
  offsetObj = fetchPointerofObject(4U, cursorObj);
  if (!(((((((extentX = fetchPointerofObject(1U, cursorObj)))) & 7) == 1)) &&
        (((((((extentY = fetchPointerofObject(2U, cursorObj)))) & 7) == 1)) &&
         (((((((depth = fetchPointerofObject(3U, cursorObj)))) & 7) == 1)) &&
          ((/* isPointers: */
            ((!(offsetObj & (tagMask())))) &&
            (((byteAt((void *)(offsetObj + (formatFieldByteOffset())))) &
              (formatMask())) <= 5 /* lastPointerFormat */)) &&
           ((((assert((classIndexOf(offsetObj)) >
                      (isForwardedObjectClassIndexPun())),
               numSlotsOf(offsetObj))) >= 2) &&
            (((((((offsetX = fetchPointerofObject(0U, offsetObj)))) & 7) ==
               1)) &&
             ((((((offsetY = fetchPointerofObject(1U, offsetObj)))) & 7) ==
               1))))))))) {
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
        ((((assert((classIndexOf(maskObj)) >
                   (isForwardedObjectClassIndexPun())),
            numSlotsOf(maskObj))) >= 5) &&
         (((fetchPointerofObject(1U, maskObj)) == ((((usqInt)16 << 3) | 1))) &&
          (((fetchPointerofObject(2U, maskObj)) == ((((usqInt)16 << 3) | 1))) &&
           (((fetchPointerofObject(3U, maskObj)) == ((((usqInt)1 << 3) | 1))) &&
            ((isWords((maskBitsObj = fetchPointerofObject(0U, maskObj)))) &&
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
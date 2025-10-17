/* Extracted from interp.c:12628 (function primitiveBeDisplay). */

/*	Record the system Display object in the specialObjectsTable,
        and if possible pin the display bitmap. Further, invoke ioBeDisplay
        to alow the VM to record the location, width heigth & depth of the
   bitmap. */

/* InterpreterPrimitives>>#primitiveBeDisplay */

static void primitiveBeDisplay(void) {
  sqInt bitsOop;
  void *bitsOrHandle;
  sqInt depthOop;
  sqInt heightOop;
  sqInt pinnedBits;
  sqInt rcvr;
  sqInt widthOop;

  rcvr = longAt(stackPointer);
  if (!((/* isPointers: */
         ((!(rcvr & (tagMask())))) &&
         (((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */)) &&
        (((lengthOf(rcvr)) >= 4) &&
         (((bitsOop = fetchPointerofObject(0U, rcvr)),
           ((/* isWordsOrBytes: */
             ((!(bitsOop & (tagMask())))) && (isWordsOrBytesNonImm(bitsOop))) ||
            ((((bitsOop) & 7) == 1))) &&
               (((((((widthOop = fetchPointerofObject(1U, rcvr)))) & 7) ==
                  1)) &&
                (((((((heightOop = fetchPointerofObject(2U, rcvr)))) & 7) ==
                   1)) &&
                 ((((((depthOop = fetchPointerofObject(3U, rcvr)))) & 7) ==
                   1))))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }

  /* begin splObj:put: */
  /* begin storePointer:ofObject:withValue: */
  assert(validStorePointerArgs(TheDisplay, specialObjectsOop, rcvr));
  assert(isNonImmediate(specialObjectsOop));
  if (oopisGreaterThanOrEqualTo(specialObjectsOop, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(rcvr & (tagMask())))) && (oopisLessThan(rcvr, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(specialObjectsOop + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(specialObjectsOop);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((specialObjectsOop + BaseHeaderSize) +
                     ((((usqInt)(TheDisplay) << (shiftForWord()))))),
            rcvr);
  if (((!(bitsOop & (tagMask())))) &&
      (!(((byteAt((void *)(bitsOop + (formatFieldByteOffset())))) &
          (1U << (pinnedBitByteShift()))) != 0))) {
    /* Answers 0 if memory required to pin but not enough memory available. */
    pinnedBits = pinObject(bitsOop);
    if (pinnedBits) {
      bitsOop = pinnedBits;
    }
  }
  bitsOrHandle = ((!(bitsOop & (tagMask()))) ? firstIndexableField(bitsOop)
                                             : ((void *)bitsOop));

  /* begin ioBeDisplay:width:height:depth: */
  displayBits = bitsOrHandle;
  displayWidth = (widthOop >> 3);
  displayHeight = (heightOop >> 3);
  displayDepth = (depthOop >> 3);
  ioNoteDisplayChangedwidthheightdepth(bitsOrHandle, (widthOop >> 3),
                                       (heightOop >> 3), (depthOop >> 3));
}
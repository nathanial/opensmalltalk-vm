/* Extracted from interp.c:58409 (function postGCUpdateDisplayBits). */

/*	Update the displayBits after a GC may have moved it.
        Answer if the displayBits appear valid. The wrinkle here is that the
        displayBits could be a surface handle. */

/* StackInterpreter>>#postGCUpdateDisplayBits */

static sqInt postGCUpdateDisplayBits(void) {
  void *bitsNow;
  sqInt bitsOop;
  sqInt displayObj;

  displayObj = longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                               ((((usqInt)(TheDisplay) << (shiftForWord()))))));
  if (!((/* isPointers: */
         ((!(displayObj & (tagMask())))) &&
         (((byteAt((void *)(displayObj + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */)) &&
        ((lengthOf(displayObj)) >= 4))) {
    return 0;
  }

  /* begin followOopField:ofObject: */
  bitsOop = longAt(
      (void *)((displayObj + BaseHeaderSize) + (0U << (shiftForWord()))));
  if (/* isOopForwarded: */
      isOopForwarded(bitsOop)) {
    bitsOop = fixFollowedFieldofObjectwithInitialValue(0, displayObj, bitsOop);
  }
  if ((bitsOop == nilObj) || ((((bitsOop) & 7) == 1))) {
    return 1;
  }

  /* it ain't yet set
     It's a surface; our work here is done... */
  assert((addressCouldBeObj(bitsOop)) && (isWordsOrBytes(bitsOop)));
  if (!((byteAt((void *)(bitsOop + (formatFieldByteOffset())))) &
        (1U << (pinnedBitByteShift())))) {
    if (stackPage) {
      pinObject(bitsOop);

      /* begin followOopField:ofObject: */
      bitsOop = longAt(
          (void *)((displayObj + BaseHeaderSize) + (0U << (shiftForWord()))));
      if (/* isOopForwarded: */
          isOopForwarded(bitsOop)) {
        bitsOop =
            fixFollowedFieldofObjectwithInitialValue(0, displayObj, bitsOop);
      }
    }

    /* If stackPage is nil we're snapshotting and now is not the time to pin. */
    bitsNow = firstIndexableField(bitsOop);
    if (displayBits != bitsNow) {
      displayBits = bitsNow;
      ioNoteDisplayChangedwidthheightdepth(displayBits, displayWidth,
                                           displayHeight, displayDepth);
    }
  }
  return 1;
}
/* Extracted from interp.c:53678 (function
 * followForwardedFrameContentsstackPointer). */

/*	follow pointers in the current stack frame up to theSP. */

/* StackInterpreter>>#followForwardedFrameContents:stackPointer: */

static void
followForwardedFrameContentsstackPointer(char *theFP, char *theSP) {
  sqInt oop;
  char *ptr;
  sqInt referent;
  char *toDoLimit;

  for (ptr = theSP; ptr <= (theFP + FoxReceiver); ptr += BytesPerWord) {
    oop = longAt(ptr);
    if (((!(oop & (tagMask())))) &&
        ((!((longAt((void *)(oop))) &
            ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
      longAtput(ptr, followForwarded(oop));
    }
  }
  toDoLimit = theFP + ((FoxCallerSavedIP + BytesPerWord) +
                       ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1)))
                          << (shiftForWord())))));
  for (ptr = ((theFP + FoxCallerSavedIP) + BytesPerWord); ptr <= toDoLimit;
       ptr += BytesPerWord) {
    oop = longAt(ptr);
    if (((!(oop & (tagMask())))) &&
        ((!((longAt((void *)(oop))) &
            ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
      /* begin followForwarded: */
      assert(isUnambiguouslyForwarder(oop));
      referent =
          longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
      while (/* isOopForwarded: */
             ((!(referent & (tagMask())))) &&
             ((!((longAt((void *)(referent))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
        referent = longAt(
            (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
      }
      oop = referent;
      longAtput(ptr, oop);
    }
  }

  /* If we're in a block then also follow fields in the stacked closure as it
   * may be activated again. */
  if (byteAt((theFP + FoxFrameFlags) + 3)) {
    assert(oop == (longAt(theFP + (frameStackedReceiverOffset(theFP)))));
    followForwardedObjectFieldstoDepth(oop, 0);
  }
  assert(!(isForwarded(frameMethodObject(theFP))));
  if (byteAt((theFP + FoxFrameFlags) + 2)) {
    assert(!(isForwarded(frameContext(theFP))));
  }
}
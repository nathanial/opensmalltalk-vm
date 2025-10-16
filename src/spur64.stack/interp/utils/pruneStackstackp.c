/* Extracted from interp.c:72153 (function pruneStackstackp). */

/*	Prune the stack to contain only the path, removing stacked indices
        and mapping frame pointers to contexts The issue here is that a
        GC can occur during ensureFrameIsMarried:SP:, but frame pointers
        are not valid objects. So first prune back to objects and framePointers
        as integers, and then replace frame pointers as integers by contexts. */

/* StackInterpreterPrimitives>>#pruneStack:stackp: */

static void pruneStackstackp(sqInt stack, sqInt stackp) {
  char *callerFP;
  sqInt finger;
  char *fp;
  sqInt i;
  sqInt objOrFP;
  sqInt oop;
  char *theFP;
  char *theFPAbove;
  StackPage *thePage;
  char *theSP;
  sqInt theStack;
  sqInt toDoLimit;

  finger = 1;
  for (i = 2; i < stackp; i += 2) {
    objOrFP = fetchPointerofObject(i, stack);
    if (/* couldBeFramePointer: */
        (((((usqInt)(((char *)objOrFP)))) & (BytesPerWord - 1)) == 0) &&
        ((((((usqInt)(((char *)objOrFP)))) >= (((usqInt)stackMemory))) &&
          ((((usqInt)(((char *)objOrFP)))) <= (((usqInt)pages)))))) {
      objOrFP += smallIntegerTag();
    }

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(stack)) && (!(isForwarded(stack))));
    assert(validStorePointerUncheckedArgs(finger, stack, objOrFP));
    longAtput((void *)((stack + BaseHeaderSize) +
                       ((((usqInt)(finger) << (shiftForWord()))))),
              objOrFP);
    finger += 1;
  }
  toDoLimit = (lengthOf(stack)) - 1;
  for (i = finger; i <= toDoLimit; i += 1) {
    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(stack)) && (!(isForwarded(stack))));
    assert(validStorePointerUncheckedArgs(i, stack, nilObj));
    longAtput((void *)((stack + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              nilObj);
  }
  oop = (theStack = stack);

  /* begin pushRemappableOop: */
  assert(addressCouldBeOop(oop));
  remapBuffer[(remapBufferCount += 1)] = oop;
  if (!(remapBufferCount <= RemapBufferSize)) {
    error("remapBuffer overflow");
  }
  for (i = 1; i < finger; i += 1) {
    objOrFP = fetchPointerofObject(i, theStack);
    if ((((objOrFP) & 7) == 1)) {
      /* begin withoutSmallIntegerTags: */
      assert((((objOrFP) & 7) == 1));
      theFP = ((char *)(objOrFP - (smallIntegerTag())));

      /* begin stackPageFor: */
      thePage = stackPageAtpages(
          pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage),
          pages);
      callerFP = ((char *)0);
      fp = (thePage->headFP);
      if (fp == theFP) {
        theFPAbove = 0;
        goto l1;
      }
      while (((callerFP = ((char *)(longAt(fp + FoxSavedFP))))) != 0) {
        if (callerFP == theFP) {
          theFPAbove = fp;
          goto l1;
        }
        fp = callerFP;
      }
      error("did not find theFP in stack page");
      theFPAbove = 0;
      /* end findFrameAbove:inPage: */
    l1:

      /* begin frameCallerSP: */
      assert(!(isBaseFrame(theFPAbove)));
      theSP =
          (theFPAbove + ((FoxCallerSavedIP + BytesPerWord) +
                         ((((usqInt)((byteAt((theFPAbove + FoxFrameFlags) + 1)))
                            << (shiftForWord())))))) +
          BytesPerWord;

      /* begin ensureFrameIsMarried:SP: */
      if (byteAt((theFP + FoxFrameFlags) + 2)) {
        assert(isContext(frameContext(theFP)));
        objOrFP = longAt(theFP + FoxThisContext);
        goto l2;
      }
      objOrFP = marryFrameSP(theFP, theSP);
      /* end ensureFrameIsMarried:SP: */
    l2:
      theStack = remapBuffer[remapBufferCount];

      /* after a GC stack may no longer be a root. */

      storePointerofObjectwithValue(finger, theStack, objOrFP);
    }
  }

  /* begin popRemappableOop */
  oop = remapBuffer[remapBufferCount];
  remapBufferCount -= 1;
}
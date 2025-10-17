/* Extracted from interp.c:51850 (function divorceFramesIn). */

/* StackInterpreter>>#divorceFramesIn: */

static void divorceFramesIn(StackPage *aStackPage) {
  sqInt calleeContext;
  char *calleeFP;
  sqInt theContext;
  char *theFP;
  sqInt theIP;
  char *theSP;
  sqInt valuePointer;

  calleeFP = ((char *)0);
  statStackPageDivorce += 1;
  theFP = (aStackPage->headFP);
  theSP = (aStackPage->headSP);
  theIP = longAt(theSP);

  /* theSP points at hottest item on frame's stack */
  theSP += BytesPerWord;
  calleeContext = null;
  while (1) {
    /* begin ensureFrameIsMarried:SP: */
    if (byteAt((theFP + FoxFrameFlags) + 2)) {
      assert(isContext(frameContext(theFP)));
      theContext = longAt(theFP + FoxThisContext);
      goto l1;
    }
    theContext = marryFrameSP(theFP, theSP);
    /* end ensureFrameIsMarried:SP: */
  l1:
    updateStateOfSpouseContextForFrameWithSP(theFP, theSP);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(theContext)) && (!(isForwarded(theContext))));
    assert(validStorePointerUncheckedArgs(
        InstructionPointerIndex, theContext,
        (/* begin contextInstructionPointer:frame: */
         assert(validInstructionPointerinFrame(theIP + 1, theFP)),
         (((usqInt)(((theIP - (longAt(theFP + FoxMethod))) - BaseHeaderSize) +
                    2)
           << 3) |
          1))));
    longAtput(
        (void *)((theContext + BaseHeaderSize) +
                 ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),
        (/* begin contextInstructionPointer:frame: */
         assert(validInstructionPointerinFrame(theIP + 1, theFP)),
         (((usqInt)(((theIP - (longAt(theFP + FoxMethod))) - BaseHeaderSize) +
                    2)
           << 3) |
          1)));
    assert((frameReceiver(theFP)) ==
           (followFieldofObject(ReceiverIndex, theContext)));
    if (calleeContext) {
      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(SenderIndex, calleeContext, theContext));
      assert(isNonImmediate(calleeContext));
      if (oopisGreaterThanOrEqualTo(calleeContext, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(theContext & (tagMask())))) &&
            (oopisLessThan(theContext, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt((void *)(calleeContext + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(calleeContext);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((calleeContext + BaseHeaderSize) +
                         ((((usqInt)(SenderIndex) << (shiftForWord()))))),
                theContext);
    }
    calleeContext = theContext;
    calleeFP = theFP;
    theIP = longAt(theFP + FoxCallerSavedIP);
    theFP = ((char *)(longAt(theFP + FoxSavedFP)));
    if (!(theFP != 0))
      break;
    /* begin frameCallerSP: */
    assert(!(isBaseFrame(calleeFP)));
    theSP = (calleeFP + ((FoxCallerSavedIP + BytesPerWord) +
                         ((((usqInt)((byteAt((calleeFP + FoxFrameFlags) + 1)))
                            << (shiftForWord())))))) +
            BytesPerWord;
  }

  /* theSP points at stacked hottest item on frame's stack */
  /* begin frameCallerContext: */
  assert(isBaseFrame(calleeFP));
  valuePointer = longAt(calleeFP + FoxCallerContext);

  /* begin storePointer:ofObject:withValue: */
  assert(validStorePointerArgs(SenderIndex, theContext, valuePointer));
  assert(isNonImmediate(theContext));
  if (oopisGreaterThanOrEqualTo(theContext, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(valuePointer & (tagMask())))) &&
        (oopisLessThan(valuePointer, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(theContext + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(theContext);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((theContext + BaseHeaderSize) +
                     ((((usqInt)(SenderIndex) << (shiftForWord()))))),
            valuePointer);

  /* The page is now free; mark it so. */
  (aStackPage->baseFP = 0);
}
/* Extracted from interp.c:68360 (function primitiveFindNextUnwindContext). */

/*	Primitive. Search up the context stack for the next method context
   marked for unwind handling from the receiver up to but not including the
   argument. Return nil if none found.
 */

/* StackInterpreterPrimitives>>#primitiveFindNextUnwindContext */

static void primitiveFindNextUnwindContext(void) {
  sqInt calleeContext;
  sqInt handlerOrNilOrZero;
  sqInt senderContext;
  sqInt senderOop;
  char *sp;
  sqInt startContext;
  sqInt stopContext;
  char *theFP;
  char *theFP1;
  char *theFPAbove;
  sqInt theMethod;
  char *theSP;

  stopContext = longAt(stackPointer);
  calleeContext = longAt(stackPointer + (1 * BytesPerWord));
  if (!((stopContext == nilObj) ||
        (/* isContext: */
         ((!(stopContext & (tagMask())))) &&
         (((longAt((void *)(stopContext))) & (classIndexMask())) ==
          ClassMethodContextCompactIndex)))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

  /* The following should never be true, but developing full blocks, early in
     September 2016 we were seeing invalid invocations of this primitive.. Hence
     the assert: */
  assert(stopContext != calleeContext);

  /* begin externalWriteBackHeadFramePointers */
  assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
  assert(stackPage == (mostRecentlyUsedPage));
  assert(!((isFree(stackPage))));

  /* begin setHeadFP:andSP:inPage: */
  assert(stackPointer < framePointer);
  assert((stackPointer < ((stackPage->baseAddress))) &&
         (stackPointer >
          (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
  assert((framePointer < ((stackPage->baseAddress))) &&
         (framePointer > (((stackPage->realStackLimit)) -
                          ((LargeContextSlots * BytesPerOop) / 2))));
  (stackPage->headFP = framePointer);
  (stackPage->headSP = stackPointer);
  assert(pageListIsWellFormed());
  if (/* isStillMarriedContext: */
      (((((longAt((void *)((calleeContext + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
         7) == 1)) &&
      (!(isWidowedContext(calleeContext)))) {
    /* begin frameOfMarriedContext: */
    senderOop =
        longAt((void *)((calleeContext + BaseHeaderSize) +
                        ((((usqInt)(SenderIndex) << (shiftForWord()))))));
    assert((((senderOop) & 7) == 1));
    theFP = ((char *)(senderOop - (smallIntegerTag())));
    if (longAt(theFP + FoxSavedFP)) {
      /* begin findMethodWithPrimitive:FromFP:UpToContext: */
      theFP1 = ((char *)(longAt(theFP + FoxSavedFP)));
      theFPAbove = ((char *)(longAt(theFP + FoxSavedFP)));
      do {
        if (((byteAt((theFP1 + FoxFrameFlags) + 2)) != 0) &&
            (stopContext == (longAt(theFP1 + FoxThisContext)))) {
          handlerOrNilOrZero = 0;
          goto l1;
        }
        if (!(byteAt((theFP1 + FoxFrameFlags) + 3))) {
          theMethod = longAt(theFP1 + FoxMethod);
          if ((primitiveIndexOf(theMethod)) == PrimNumberUnwindMarker) {
            if (theFP1 == theFPAbove) {
              theSP = findSPOfon(theFP1, stackPageFor(theFP1));
            } else {
              /* begin frameCallerStackPointer: */
              assert(!(isBaseFrame(theFPAbove)));
              theSP = (theFPAbove +
                       ((FoxCallerSavedIP + BytesPerWord) +
                        ((((usqInt)((byteAt((theFPAbove + FoxFrameFlags) + 1)))
                           << (shiftForWord())))))) +
                      BytesPerWord;
            }

            /* begin ensureFrameIsMarried:SP: */
            if (byteAt((theFP1 + FoxFrameFlags) + 2)) {
              assert(isContext(frameContext(theFP1)));
              handlerOrNilOrZero = longAt(theFP1 + FoxThisContext);
              goto l1;
            }
            handlerOrNilOrZero = marryFrameSP(theFP1, theSP);
            goto l1;
          }
        }
        theFPAbove = theFP1;
        theFP1 = ((char *)(longAt(theFP1 + FoxSavedFP)));
      } while (theFP1 != 0);

      /* begin frameCallerContext: */
      assert(isBaseFrame(theFPAbove));
      senderContext = longAt(theFPAbove + FoxCallerContext);
      if (!(/* isContext: */
            ((!(senderContext & (tagMask())))) &&
            (((longAt((void *)(senderContext))) & (classIndexMask())) ==
             ClassMethodContextCompactIndex))) {
        handlerOrNilOrZero = nilObj;
        goto l1;
      }
      handlerOrNilOrZero = findMethodWithPrimitiveFromContextUpToContext(
          PrimNumberUnwindMarker, senderContext, stopContext);
      /* end findMethodWithPrimitive:FromFP:UpToContext: */
    l1:;
    } else {
      handlerOrNilOrZero = findMethodWithPrimitiveFromContextUpToContext(
          PrimNumberUnwindMarker,
          (/* begin frameCallerContext: */
           assert(isBaseFrame(theFP)), longAt(theFP + FoxCallerContext)),
          stopContext);
    }
  } else {
    startContext =
        longAt((void *)((calleeContext + BaseHeaderSize) +
                        ((((usqInt)(SenderIndex) << (shiftForWord()))))));
    if (/* isContext: */
        ((!(startContext & (tagMask())))) &&
        (((longAt((void *)(startContext))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex)) {
      handlerOrNilOrZero = findMethodWithPrimitiveFromContextUpToContext(
          PrimNumberUnwindMarker, startContext, stopContext);
    } else {
      handlerOrNilOrZero = 0;
    }
  }
  if (!handlerOrNilOrZero) {
    handlerOrNilOrZero = nilObj;
  }

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer + (1 * BytesPerWord)), handlerOrNilOrZero);
  stackPointer = sp;
}
/* Extracted from interp.c:70247 (function primitiveTerminateTo). */

/*	Primitive. Terminate up the context stack from the receiver up to but
   not including the argument, if previousContext is on my Context stack. Make
        previousContext my
        sender. This prim has to shadow the code in ContextPart>terminateTo: to
   be correct.
 */

/* StackInterpreterPrimitives>>#primitiveTerminateTo */

static void primitiveTerminateTo(void) {
  sqInt aContextOrNil;
  char *callerFP;
  char *callerFP1;
  char *contextsFP;
  usqInt contextsIP;
  char *contextsSP;
  sqInt currentCtx;
  char *fp;
  char *fp1;
  char *frameAbove;
  sqInt handlerOrNilOrZero;
  char *newFP;
  char *newSP;
  sqInt nextCntx;
  StackPage *pageToStopOn;
  sqInt senderOop;
  sqInt senderOopSqInt;
  char *source;
  sqInt stackedReceiverOffset;
  char *theFP;
  StackPage *thePage;
  sqInt thisCtx;
  sqInt valuePointer;

  contextsFP = ((char *)0);
  aContextOrNil = longAt(stackPointer);
  if (!((aContextOrNil == nilObj) ||
        (/* isContext: */
         ((!(aContextOrNil & (tagMask())))) &&
         (((longAt((void *)(aContextOrNil))) & (classIndexMask())) ==
          ClassMethodContextCompactIndex)))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  thisCtx = longAt(stackPointer + (1 * BytesPerWord));
  if (thisCtx == aContextOrNil) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

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

  /* If we're searching for aContextOrNil it might be on a stack page.  Helps to
     know if we can free a whole page or not, or if we can short-cut the
     termination. */
  if ((aContextOrNil != nilObj) &&
      (/* isStillMarriedContext: */
       (((((fetchPointerofObject(SenderIndex, aContextOrNil))) & 7) == 1)) &&
       (!(isWidowedContext(aContextOrNil))))) {
    /* begin frameOfMarriedContext: */
    senderOop = fetchPointerofObject(SenderIndex, aContextOrNil);
    assert((((senderOop) & 7) == 1));
    contextsFP = ((char *)(senderOop - (smallIntegerTag())));

    /* begin stackPageFor: */
    pageToStopOn = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                        contextsFP, stackMemory, bytesPerPage),
                                    pages);
  } else {
    pageToStopOn = 0;
  }

  /* if thisCtx is married ensure it is a base frame.  Then we can assign its
   * sender. */
  if (/* isStillMarriedContext: */
      (((((fetchPointerofObject(SenderIndex, thisCtx))) & 7) == 1)) &&
      (!(isWidowedContext(thisCtx)))) {
    /* begin frameOfMarriedContext: */
    senderOop = fetchPointerofObject(SenderIndex, thisCtx);
    assert((((senderOop) & 7) == 1));
    theFP = ((char *)(senderOop - (smallIntegerTag())));

    /* Optimize terminating thisContext.  Move its frame down to be next to
       aContextOrNil's frame. Common in the exception system and so helps to be
       fast. */
    if ((theFP == framePointer) && (pageToStopOn == stackPage)) {
      if ((((char *)(longAt(theFP + FoxSavedFP)))) != contextsFP) {
        stackedReceiverOffset =
            (FoxCallerSavedIP + BytesPerWord) +
            ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1)))
               << (shiftForWord()))));

        /* begin findFrameAbove:inPage: */
        callerFP = ((char *)0);
        fp = (pageToStopOn->headFP);
        if (fp == contextsFP) {
          frameAbove = 0;
          goto l1;
        }
        while (((callerFP = ((char *)(longAt(fp + FoxSavedFP))))) != 0) {
          if (callerFP == contextsFP) {
            frameAbove = fp;
            goto l1;
          }
          fp = callerFP;
        }
        error("did not find theFP in stack page");
        frameAbove = 0;
        /* end findFrameAbove:inPage: */
      l1:
        contextsIP = longAt(frameAbove + FoxCallerSavedIP);

        /* begin frameCallerSP: */
        assert(!(isBaseFrame(frameAbove)));
        newSP = (frameAbove +
                 ((FoxCallerSavedIP + BytesPerWord) +
                  ((((usqInt)((byteAt((frameAbove + FoxFrameFlags) + 1)))
                     << (shiftForWord())))))) +
                BytesPerWord;
        newFP = (newSP - stackedReceiverOffset) - BytesPerWord;
        for (source = (theFP + stackedReceiverOffset); source >= stackPointer;
             source += (-BytesPerWord)) {
          newSP -= BytesPerWord;
          longAtput(newSP, longAt(source));
        }
        longAtput(newFP + FoxSavedFP, ((usqInt)contextsFP));
        longAtput(newFP + FoxCallerSavedIP, contextsIP);
        assert(isContext(thisCtx));
        /* begin withSmallIntegerTags: */
        assert(((oopForPointer(newFP)) & (BytesPerWord - 1)) == 0);
        valuePointer = (oopForPointer(newFP)) + (smallIntegerTag());

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(thisCtx)) && (!(isForwarded(thisCtx))));
        assert(
            validStorePointerUncheckedArgs(SenderIndex, thisCtx, valuePointer));
        longAtput((void *)((thisCtx + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord()))))),
                  valuePointer);
        /* begin withSmallIntegerTags: */
        assert(((oopForPointer(contextsFP)) & (BytesPerWord - 1)) == 0);
        valuePointer = (oopForPointer(contextsFP)) + (smallIntegerTag());

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(thisCtx)) && (!(isForwarded(thisCtx))));
        assert(validStorePointerUncheckedArgs(InstructionPointerIndex, thisCtx,
                                              valuePointer));
        longAtput((void *)((thisCtx + BaseHeaderSize) +
                           ((((usqInt)(InstructionPointerIndex)
                              << (shiftForWord()))))),
                  valuePointer);
        framePointer = newFP;
        stackPointer = newSP;
      }

      /* begin pop: */
      stackPointer += 1 * BytesPerWord;
      assert(stackPage == (mostRecentlyUsedPage));
      return;
    }

    /* May cause a GC!! */
    theFP = externalEnsureIsBaseFrame(theFP);

    /* begin frameCallerContext: */
    assert(isBaseFrame(theFP));
    currentCtx = longAt(theFP + FoxCallerContext);

    /* May also reclaim aContextOrNil's page, hence... */
    if ((aContextOrNil != nilObj) &&
        (/* isStillMarriedContext: */
         (((((fetchPointerofObject(SenderIndex, aContextOrNil))) & 7) == 1)) &&
         (!(isWidowedContext(aContextOrNil))))) {
      /* begin frameOfMarriedContext: */
      senderOop = fetchPointerofObject(SenderIndex, aContextOrNil);
      assert((((senderOop) & 7) == 1));
      contextsFP = ((char *)(senderOop - (smallIntegerTag())));

      /* begin stackPageFor: */
      pageToStopOn =
          stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                               contextsFP, stackMemory, bytesPerPage),
                           pages);
    } else {
      pageToStopOn = 0;
    }
  } else {
    currentCtx = fetchPointerofObject(SenderIndex, thisCtx);
  }

  /* begin context:hasSender: */
  handlerOrNilOrZero =
      findMethodWithPrimitiveFromContextUpToContext(-1, thisCtx, aContextOrNil);
  if (!handlerOrNilOrZero) {
    while (!((currentCtx == aContextOrNil) || (currentCtx == nilObj))) {
      assert(isContext(currentCtx));
      if (((((fetchPointerofObject(SenderIndex, currentCtx))) & 7) == 1)) {
        /* begin frameOfMarriedContext: */
        senderOop = fetchPointerofObject(SenderIndex, currentCtx);
        assert((((senderOop) & 7) == 1));
        theFP = ((char *)(senderOop - (smallIntegerTag())));

        /* begin stackPageFor: */
        thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                       theFP, stackMemory, bytesPerPage),
                                   pages);

        /* If externalEnsureIsBaseFrame: above has moved thisContext to its own
           stack then we will always terminate to a frame on a different page.
           But if we are terminating some other context to a context somewhere
           on the current page we must save the active frames above that
           context.  Things will look e.g. like this: thisCtx
           499383332 s MethodContext(ContextPart)>resume: 499380484 s
           BlockClosure>ensure: 499377320 s
           MethodContext(ContextPart)>handleSignal: 499373760 s
           MethodContext(ContextPart)>handleSignal: 499372772 s
           MessageNotUnderstood(Exception)>signal 499369068 s
           CodeSimulationTests(Object)>doesNotUnderstand: absentMethod 499368708
           s [] in CodeSimulationTests>testDNU (sender is 0xbffc2480 I
           CodeSimulationTests>runSimulated:)
           ------------
           framePointer	0xbffc234c M
           MethodContext(ContextPart)>doPrimitive:method:receiver:args:
           0xbffc2378 M
           MethodContext(ContextPart)>tryPrimitiveFor:receiver:args: 0xbffc23ac
           M MethodContext(ContextPart)>send:to:with:super: 0xbffc23e4 M
           MethodContext(ContextPart)>send:super:numArgs: 0xbffc2418 M
           MethodContext(InstructionStream)>interpretNextInstructionFor:
           0xbffc2434 M MethodContext(ContextPart)>step
           0xbffc2458 I
           MethodContext(ContextPart)>runSimulated:contextAtEachStep:
           ------------
           (499368708's sender)	0xbffc2480 I CodeSimulationTests>runSimulated:
           0xbffc249c M CodeSimulationTests>testDNU
           0xbffc24bc I CodeSimulationTests(TestCase)>performTest
           0xbffc24dc I [] in CodeSimulationTests(TestCase)>runCase
           aContextOrNil	0xbffc24fc M BlockClosure>ensure:
           0xbffc2520 I CodeSimulationTests(TestCase)>runCase
           0xbffc253c M [] in TestResult>runCase:
           When we find this case we move the frames above to a new page by
           making the frame above currentCtx a base frame, i.e. making
           0xbffc2458 in the above example a base frame.  But in this iteration
           of the loop we don't move down a frame i.e. currentCtx doesn't change
           on this iteration. */
        if (thePage == stackPage) {
          /* begin findFrameAbove:inPage: */
          callerFP = ((char *)0);
          fp = (thePage->headFP);
          if (fp == theFP) {
            frameAbove = 0;
            goto l2;
          }
          while (((callerFP = ((char *)(longAt(fp + FoxSavedFP))))) != 0) {
            if (callerFP == theFP) {
              frameAbove = fp;
              goto l2;
            }
            fp = callerFP;
          }
          error("did not find theFP in stack page");
          frameAbove = 0;
          /* end findFrameAbove:inPage: */
        l2:
          assert(frameAbove != 0);

          /* May cause a GC!! May also reclaim aContextOrNil's page, hence... */
          frameAbove = externalEnsureIsBaseFrame(frameAbove);
          if ((aContextOrNil != nilObj) &&
              (/* isStillMarriedContext: */
               (((((fetchPointerofObject(SenderIndex, aContextOrNil))) & 7) ==
                 1)) &&
               (!(isWidowedContext(aContextOrNil))))) {
            /* begin frameOfMarriedContext: */
            senderOopSqInt = fetchPointerofObject(SenderIndex, aContextOrNil);
            assert((((senderOopSqInt) & 7) == 1));
            contextsFP = ((char *)(senderOopSqInt - (smallIntegerTag())));

            /* begin stackPageFor: */
            pageToStopOn =
                stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                     contextsFP, stackMemory, bytesPerPage),
                                 pages);
          } else {
            pageToStopOn = 0;
          }
        } else {
          if (thePage == pageToStopOn) {
            /* begin findFrameAbove:inPage: */
            callerFP1 = ((char *)0);
            fp1 = (thePage->headFP);
            if (fp1 == contextsFP) {
              frameAbove = 0;
              goto l3;
            }
            while (((callerFP1 = ((char *)(longAt(fp1 + FoxSavedFP))))) != 0) {
              if (callerFP1 == contextsFP) {
                frameAbove = fp1;
                goto l3;
              }
              fp1 = callerFP1;
            }
            error("did not find theFP in stack page");
            frameAbove = 0;
            /* end findFrameAbove:inPage: */
          l3:
            if (frameAbove) {
              contextsSP =
                  ((/* begin frameCallerSP: */
                    assert(!(isBaseFrame(frameAbove))),
                    (frameAbove +
                     ((FoxCallerSavedIP + BytesPerWord) +
                      ((((usqInt)((byteAt((frameAbove + FoxFrameFlags) + 1)))
                         << (shiftForWord())))))) +
                        BytesPerWord)) -
                  BytesPerWord;
              longAtput(contextsSP, longAt(frameAbove + FoxCallerSavedIP));

              /* begin setHeadFP:andSP:inPage: */
              assert(contextsSP < contextsFP);
              assert((contextsSP < ((thePage->baseAddress))) &&
                     (contextsSP > (((thePage->realStackLimit)) -
                                    (LargeContextSlots * BytesPerOop))));
              assert((contextsFP < ((thePage->baseAddress))) &&
                     (contextsFP > (((thePage->realStackLimit)) -
                                    ((LargeContextSlots * BytesPerOop) / 2))));
              (thePage->headFP = contextsFP);
              (thePage->headSP = contextsSP);
            }
            currentCtx = aContextOrNil;
          } else {
            /* begin frameCallerContext: */
            assert(isBaseFrame((thePage->baseFP)));
            currentCtx = longAt(((thePage->baseFP)) + FoxCallerContext);

            /* for a short time invariant is violated; assert follows */
            freeStackPageNoAssert(thePage);
          }
        }
      } else {
        nextCntx = fetchPointerofObject(SenderIndex, currentCtx);

        /* begin markContextAsDead: */
        assert(isContext(currentCtx));
        assert((isNonImmediate(currentCtx)) && (!(isForwarded(currentCtx))));
        assert(validStorePointerUncheckedArgs(SenderIndex, currentCtx, nilObj));
        longAtput((void *)((currentCtx + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord()))))),
                  nilObj);

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(currentCtx)) && (!(isForwarded(currentCtx))));
        assert(validStorePointerUncheckedArgs(InstructionPointerIndex,
                                              currentCtx, nilObj));
        longAtput((void *)((currentCtx + BaseHeaderSize) +
                           ((((usqInt)(InstructionPointerIndex)
                              << (shiftForWord()))))),
                  nilObj);
        currentCtx = nextCntx;
      }
    }
  }

  /* Need to walk the stack freeing stack pages and nilling contexts. */
  assert(pageListIsWellFormed());
  if (((((fetchPointerofObject(SenderIndex, thisCtx))) & 7) == 1)) {
    assert(checkIsStillMarriedContextcurrentFP(thisCtx, framePointer));
    assert(isBaseFrame(frameOfMarriedContext(thisCtx)));

    /* begin frameOfMarriedContext: */
    senderOop = fetchPointerofObject(SenderIndex, thisCtx);
    assert((((senderOop) & 7) == 1));
    theFP = ((char *)(senderOop - (smallIntegerTag())));

    /* begin frameCallerContext:put: */
    assert(isBaseFrame(theFP));
    assert((aContextOrNil == (nilObject())) || (isContext(aContextOrNil)));
    longAtput(theFP + FoxCallerContext, aContextOrNil);
  } else {
    /* begin storePointer:ofObject:withValue: */
    assert(validStorePointerArgs(SenderIndex, thisCtx, aContextOrNil));
    assert(isNonImmediate(thisCtx));
    if (oopisGreaterThanOrEqualTo(thisCtx, oldSpaceStart)) {
      if (/* isYoung: */
          ((!(aContextOrNil & (tagMask())))) &&
          (oopisLessThan(aContextOrNil, oldSpaceStart))) {
        /* begin possibleRootStoreInto: */
        if (!((byteAt((void *)(thisCtx + (formatFieldByteOffset())))) &
              (1U << (rememberedBitByteShift())))) {
          remember(thisCtx);
        }
      }
    }

    /* most stores into young objects */
    longAtput((void *)((thisCtx + BaseHeaderSize) +
                       ((((usqInt)(SenderIndex) << (shiftForWord()))))),
              aContextOrNil);
  }

  /* begin pop: */
  stackPointer += 1 * BytesPerWord;
  assert(stackPage == (mostRecentlyUsedPage));
}
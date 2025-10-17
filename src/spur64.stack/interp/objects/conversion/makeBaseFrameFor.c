/* Extracted from interp.c:56551 (function makeBaseFrameFor). */

/*	<Integer> */
/*	Marry aContext with the base frame of a new stack page. Build the base
        frame to reflect the context's state. Answer the new page. */

/* StackInterpreter>>#makeBaseFrameFor: */

static StackPage *makeBaseFrameFor(sqInt aContext) {
  sqInt header;
  sqInt i;
  sqInt maybeClosure;
  usqInt numArgs;
  sqInt oop;
  StackPage *page;
  char *pointer;
  sqInt rcvr;
  sqInt stackPtrIndex;
  sqInt theIP;
  sqInt theMethod;
  sqInt valuePointer;

  assert(isContext(aContext));
  assert(isSingleContext(aContext));
  assert(goodContextSize(aContext));

  /* begin newStackPage */
  page = (mostRecentlyUsedPage->nextPage);
  if (!((page->baseFP))) {
    goto l1;
  }
  divorceFramesIn(page);
  /* end newStackPage */
l1:
  pointer = (page->baseAddress);
  theIP = fetchPointerofObject(InstructionPointerIndex, aContext);

  /* begin followObjField:ofObject: */
  theMethod = fetchPointerofObject(MethodIndex, aContext);
  assert(isNonImmediate(theMethod));
  if ((!((longAt((void *)(theMethod))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    theMethod = fixFollowedFieldofObjectwithInitialValue(MethodIndex, aContext,
                                                         theMethod);
  }
  if (!((((theIP) & 7) == 1))) {
    error("context is not resumable");
  }
  theIP = (theIP >> 3);

  /* begin followField:ofObject: */
  rcvr = fetchPointerofObject(ReceiverIndex, aContext);
  if (isOopForwarded(rcvr)) {
    rcvr =
        fixFollowedFieldofObjectwithInitialValue(ReceiverIndex, aContext, rcvr);
  }

  /* If the frame is a closure activation then the closure should be on the
     stack in the pushed receiver position (closures receive the value[:value:]
     messages). Otherwise it should be the receiver proper. */
  maybeClosure =
      fetchPointerofObject(ClosureIndex, aContext);
  if (maybeClosure != nilObj) {
    if ((!((longAt((void *)(maybeClosure))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      maybeClosure = fixFollowedFieldofObjectwithInitialValue(
          ClosureIndex, aContext, maybeClosure);
    }

    /* begin argumentCountOfClosure: */
    /* begin quickFetchInteger:ofObject: */
    oop = fetchPointerofObject(ClosureNumArgsIndex, maybeClosure);
    assert((((oop) & 7) == 1));
    numArgs = (oop >> 3);
    longAtput(pointer, maybeClosure);
  } else {
    /* begin methodHeaderOf: */
    assert(isCompiledMethod(theMethod));
    header = fetchPointerofObject(HeaderIndex, theMethod);
    numArgs = (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;

    /* If this is a synthetic context its IP could be pointing at the
     * CallPrimitive opcode.  If so, skip it. */
    if ((((header & AlternateHeaderHasPrimFlag) != 0)) &&
        (theIP == (1 + ((((/* begin literalCountOfMethodHeader: */
                           assert((((header) & 7) == 1)),
                           /* literalCountOfAlternateHeader: */
                           ((header >> 3)) & AlternateHeaderNumLiteralsMask)) +
                         LiteralStart) *
                        BytesPerOop)))) {
      theIP += 3 /* sizeOfCallPrimitiveBytecode: */;
    }
    longAtput(pointer, rcvr);
  }

  /* Put the arguments on the stack */
  for (i = 1; i <= numArgs; i += 1) {
    longAtput((pointer -= BytesPerWord),
              fetchPointerofObject(ReceiverIndex + i, aContext));

    /* nil the slot in the context so that it doesn't inadvertently hang onto
       some collectable object. Thanks to Ryan Macnak for identifying this bug
     */

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(aContext)) && (!(isForwarded(aContext))));
    assert(validStorePointerUncheckedArgs(ReceiverIndex + i, aContext, nilObj));
    longAtput((void *)((aContext + BaseHeaderSize) +
                       ((((usqInt)((ReceiverIndex + i)) << (shiftForWord()))))),
              nilObj);
  }

  /* saved caller ip is sender context in base frame */
  longAtput((pointer -= BytesPerWord),
            followObjFieldofObject(SenderIndex, aContext));

  /* base frame's saved fp is null */
  longAtput((pointer -= BytesPerWord), 0);
  (page->baseFP = pointer);
  (page->headFP = pointer);
  longAtput((pointer -= BytesPerWord), theMethod);
  longAtput((pointer -=
             BytesPerWord), /* encodeFrameFieldHasContext:isBlock:numArgs: */
            (VMBIGENDIAN ? ((1 + ((numArgs << ((BytesPerWord * 8) - 8)))) +
                            (1ULL << ((BytesPerWord * 8) - 16))) +
                               ((maybeClosure != nilObj
                                     ? 1ULL << ((BytesPerWord * 8) - 24)
                                     : 0))
                         : ((1 + ((numArgs << 8))) + (0x10000)) +
                               ((maybeClosure != nilObj ? 0x1000000 : 0))));
  assert(frameHasContext((page->baseFP)));
  assert((frameNumArgs((page->baseFP))) == numArgs);
  longAtput((pointer -= BytesPerWord), aContext);
  longAtput((pointer -= BytesPerWord), rcvr);

  /* begin quickFetchInteger:ofObject: */
  oop = fetchPointerofObject(StackPointerIndex, aContext);
  assert((((oop) & 7) == 1));
  stackPtrIndex = (oop >> 3);
  assert((ReceiverIndex + stackPtrIndex) < (lengthOf(aContext)));
  for (i = (numArgs + 1); i <= stackPtrIndex; i += 1) {
    longAtput((pointer -= BytesPerWord),
              fetchPointerofObject(ReceiverIndex + i, aContext));
  }

  /* top of stack is the instruction pointer */

  /* begin iframeInstructionPointerForIndex:method: */
  assert(
      ((theIP >=
        (((LiteralStart + (literalCountOf(theMethod))) * BytesPerOop) + 1)) &&
       (theIP <= (lengthOf(theMethod)))));
  theIP = ((theMethod + theIP) + BaseHeaderSize) - 2;
  longAtput((pointer -= BytesPerWord), theIP);
  (page->headSP = pointer);
  assert(contexthasValidInversePCMappingOfin(aContext, theIP, (page->baseFP)));

  /* Mark context as married by setting its sender to the frame pointer plus
     SmallInteger tags and the InstructionPointer to the saved fp (which ensures
     correct alignment w.r.t. the frame when we check for validity) plus
     SmallInteger tags. */
  /* begin withSmallIntegerTags: */
  assert(((oopForPointer((page->baseFP))) & (BytesPerWord - 1)) == 0);
  valuePointer = (oopForPointer((page->baseFP))) + (smallIntegerTag());

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(aContext)) && (!(isForwarded(aContext))));
  assert(validStorePointerUncheckedArgs(SenderIndex, aContext, valuePointer));
  longAtput((void *)((aContext + BaseHeaderSize) +
                     ((((usqInt)(SenderIndex) << (shiftForWord()))))),
            valuePointer);
  /* begin withSmallIntegerTags: */
  assert(((oopForPointer(0)) & (BytesPerWord - 1)) == 0);
  valuePointer = (oopForPointer(0)) + (smallIntegerTag());

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(aContext)) && (!(isForwarded(aContext))));
  assert(validStorePointerUncheckedArgs(InstructionPointerIndex, aContext,
                                        valuePointer));
  longAtput(
      (void *)((aContext + BaseHeaderSize) +
               ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),
      valuePointer);
  assert(((((fetchPointerofObject(SenderIndex, aContext))) & 7) == 1));
  assert((frameOfMarriedContext(aContext)) == ((page->baseFP)));
  assert(validStackPageBaseFrame(page));
  return page;
}
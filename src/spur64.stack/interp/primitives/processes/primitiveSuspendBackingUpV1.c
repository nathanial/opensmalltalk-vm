/* Extracted from interp.c:70119 (function primitiveSuspendBackingUpV1). */

/*	Primitive #568. Suspend the receiver, aProcess, such that it can be
        executed again
        by sending #resume. If the given process is not the active process, take
        it off
        its corresponding list. If the list was not its run queue assume it was
   on some condition variable (Semaphore, Mutex) and back up its pc to the send
        that invoked the wait state the process entered. Hence when the process
        resumes it will reenter the wait state. Answer the list the receiver was
        previously on,
        unless it was the activeProcess, in which case answer nil.
        c.f. primitiveSuspend,#88 & primitiveSuspendBackingUpV2,#578 */

/* StackInterpreterPrimitives>>#primitiveSuspendBackingUpV1 */

static void primitiveSuspendBackingUpV1(void) {
  sqInt myContext;
  sqInt myList;
  usqInt numSlots;
  sqInt ok;
  sqInt process;

  process = longAt(stackPointer);
  if (process == (activeProcess())) {
    /* stackTopPut: */
    longAtput(stackPointer, nilObj);

    /* transferToHighestPriorityProcessFrom: */
    transferTofrom(wakeHighestPriority(), CSSuspend);
    return;
  }
  myList = fetchPointerofObject(MyListIndex, process);
  myContext = fetchPointerofObject(SuspendedContextIndex, process);
  if (!((/* isPointers: */
         ((!(myList & (tagMask())))) &&
         (((byteAt((void *)(myList + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */)) &&
        ((((
            numSlotsOf(myList))) > LastLinkIndex) &&
         ((((longAt((void *)(myContext))) & (classIndexMask())) ==
           ClassMethodContextCompactIndex) &&
          ((/* begin isResumableContext: */
            assert(isContext(myContext)),
            ((((fetchPointerofObject(InstructionPointerIndex, myContext))) &
              7) == 1))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  ok = removeProcessfromList(process, myList);
  if (!ok) {
    /* primitiveFailFor: */
    primFailCode = PrimErrOperationFailed;
    return;
  }
  if (((longAt((void *)(myList))) & (classIndexMask())) !=
      classLinkedListClassTag) {
    backupContexttoBlockingSendTo(myContext, myList);
  }

  /* stackTopPut: */
  longAtput(stackPointer, myList);
}
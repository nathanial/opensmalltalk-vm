/* Extracted from interp.c:25081 (function primitiveSuspend). */

/*	Primitive #88. Suspend the receiver, aProcess, such that it can be
        executed again
        by sending #resume. If the given process is not the active process, take
        it off
        its corresponding list. The primitive returns the list the receiver was
        previously on.
        c.f. primitiveSuspendBackingUpV1,#568 & primitiveSuspendBackingUpV2,#578
 */

/* InterpreterPrimitives>>#primitiveSuspend */

static void primitiveSuspend(void) {
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
        ((((numSlotsOf(myList))) > LastLinkIndex) &&
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

  /* stackTopPut: */
  longAtput(stackPointer, myList);
}
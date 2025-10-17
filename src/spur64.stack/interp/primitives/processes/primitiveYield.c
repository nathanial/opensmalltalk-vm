/* Extracted from interp.c:25893 (function primitiveYield). */

/*	Primitively do the equivalent of Process>yield, avoiding the overhead of
   a fork and a wait in the standard implementation.
 */

/* InterpreterPrimitives>>#primitiveYield */

static void primitiveYield(void) {
  sqInt activeProc;
  sqInt oop;
  sqInt priority;
  sqInt processList;
  sqInt processLists;
  sqInt scheduler;

  scheduler =
      fetchPointerofObject(ValueIndex, fetchPointerofObject(SchedulerAssociation, specialObjectsOop));
  activeProc =
      fetchPointerofObject(ActiveProcessIndex, scheduler);

  /* begin quickFetchInteger:ofObject: */
  oop = fetchPointerofObject(PriorityIndex, activeProc);
  assert((((oop) & 7) == 1));
  priority = (oop >> 3);
  processLists =
      fetchPointerofObject(ProcessListsIndex, scheduler);
  processList =
      fetchPointerofObject(priority - 1, processLists);

  /* begin isEmptyList: */
  assert(!(isForwarded(processList)));
  if (!((fetchPointerofObject(FirstLinkIndex, processList)) == nilObj)) {
    addLastLinktoList(activeProc, processList);

    /* transferToHighestPriorityProcessFrom: */
    transferTofrom(wakeHighestPriority(), CSYield);
  }
}
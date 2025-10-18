/* Extracted from interp.c:61300 (function putToSleepyieldingIf). */

/*	Save the given process on the scheduler process list for its priority,
        adding to the back if yieldImplicitly or to the front if not
        yieldImplicitly.
 */

/* StackInterpreter>>#putToSleep:yieldingIf: */

static void putToSleepyieldingIf(sqInt aProcess, sqInt yieldImplicitly) {
  sqInt firstLink;
  sqInt objOop;
  sqInt oop;
  sqInt priority;
  sqInt processList;
  sqInt processLists;

  assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));

  /* begin quickFetchInteger:ofObject: */
  oop = fetchPointerofObject(PriorityIndex, aProcess);
  assert((((oop) & 7) == 1));
  priority = (oop >> 3);
  if ((highestRunnableProcessPriority != 0) &&
      (priority > highestRunnableProcessPriority)) {
    highestRunnableProcessPriority = priority;
  }
  objOop = fetchPointerofObject(
      ValueIndex,
      fetchPointerofObject(SchedulerAssociation, specialObjectsOop));

  processLists = fetchPointerofObject(ProcessListsIndex, objOop);
  processList = fetchPointerofObject(priority - 1, processLists);
  if (yieldImplicitly) {
    addLastLinktoList(aProcess, processList);
  } else {
    /* begin addFirstLink:toList: */
    assert(!(isForwarded(processList)));
    assert((fetchPointerofObject(NextLinkIndex, aProcess)) == (nilObject()));
    firstLink = fetchPointerofObject(FirstLinkIndex, processList);
    assert(firstLink != aProcess);

    storePointerofObjectwithValue(FirstLinkIndex, processList, aProcess);
    if (firstLink == nilObj) {
      storePointerofObjectwithValue(LastLinkIndex, processList, aProcess);
    } else {
      storePointerofObjectwithValue(NextLinkIndex, aProcess, firstLink);
    }

    /* a.k.a. (self isEmptyList: aList) */

    storePointerofObjectwithValue(MyListIndex, aProcess, processList);
  }
}
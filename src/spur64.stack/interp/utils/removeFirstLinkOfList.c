/* Extracted from interp.c:62030 (function removeFirstLinkOfList). */

/*	Remove the first process from the given linked list. */

/* StackInterpreter>>#removeFirstLinkOfList: */

static sqInt removeFirstLinkOfList(sqInt aList) {
  sqInt first;
  sqInt last;
  sqInt next;

  assert(!((isForwarded(aList))));
  first = fetchPointerofObject(FirstLinkIndex, aList);
  last = fetchPointerofObject(LastLinkIndex, aList);
  assert(!((isForwarded(first))));
  assert(!((isForwarded(last))));
  if (first == last) {
    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(aList)) && (!(isForwarded(aList))));
    assert(validStorePointerUncheckedArgs(FirstLinkIndex, aList, nilObj));
    longAtput((void *)((aList + BaseHeaderSize) +
                       ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))),
              nilObj);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(aList)) && (!(isForwarded(aList))));
    assert(validStorePointerUncheckedArgs(LastLinkIndex, aList, nilObj));
    longAtput((void *)((aList + BaseHeaderSize) +
                       ((((usqInt)(LastLinkIndex) << (shiftForWord()))))),
              nilObj);
  } else {
    next = fetchPointerofObject(NextLinkIndex, first);

    storePointerofObjectwithValue(FirstLinkIndex, aList, next);
  }

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(first)) && (!(isForwarded(first))));
  assert(validStorePointerUncheckedArgs(NextLinkIndex, first, nilObj));
  longAtput((void *)((first + BaseHeaderSize) +
                     ((((usqInt)(NextLinkIndex) << (shiftForWord()))))),
            nilObj);
  return first;
}
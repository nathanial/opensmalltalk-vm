/* Extracted from interp.c:62090 (function removeProcessfromList). */

/*	Attempt to remove a process from a linked list. Answer if the attempt
        succeeded.
 */

/* StackInterpreter>>#removeProcess:fromList: */

static sqInt removeProcessfromList(sqInt aProcess, sqInt aList) {
  sqInt firstLink;
  sqInt lastLink;
  sqInt nextLink;
  sqInt tempLink;

  nextLink = 0;
  assert(!((isForwarded(aProcess))));
  assert(!((isForwarded(aList))));

  /* any process on the list could have been becomed, so use a read barrier...
   */

  firstLink = followObjFieldofObject(FirstLinkIndex, aList);

  lastLink = followObjFieldofObject(LastLinkIndex, aList);
  if (aProcess == firstLink) {
    nextLink = followObjFieldofObject(NextLinkIndex, aProcess);

    storePointerofObjectwithValue(FirstLinkIndex, aList, nextLink);
    if (aProcess == lastLink) {
      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(aList)) && (!(isForwarded(aList))));
      assert(validStorePointerUncheckedArgs(LastLinkIndex, aList, nilObj));
      longAtput((void *)((aList + BaseHeaderSize) +
                         ((((usqInt)(LastLinkIndex) << (shiftForWord()))))),
                nilObj);
    }
  } else {
    tempLink = firstLink;

    /* fail if any link doesn't look like a process... */
    while (1) {
      if (!((/* isPointers: */
             ((!(tempLink & (tagMask())))) &&
             (((byteAt((void *)(tempLink + (formatFieldByteOffset())))) &
               (formatMask())) <= 5 /* lastPointerFormat */)) &&
            (((assert((classIndexOf(tempLink)) >
                      (isForwardedObjectClassIndexPun())),
               numSlotsOf(tempLink))) > MyListIndex))) {
        return 0;
      }

      nextLink = followObjFieldofObject(NextLinkIndex, tempLink);
      if (nextLink == aProcess)
        break;
      tempLink = nextLink;
    }
    nextLink = fetchPointerofObject(NextLinkIndex, aProcess);

    storePointerofObjectwithValue(NextLinkIndex, tempLink, nextLink);
    if (aProcess == lastLink) {
      storePointerofObjectwithValue(LastLinkIndex, aList, tempLink);
    }
  }

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(aProcess)) && (!(isForwarded(aProcess))));
  assert(validStorePointerUncheckedArgs(NextLinkIndex, aProcess, nilObj));
  longAtput((void *)((aProcess + BaseHeaderSize) +
                     ((((usqInt)(NextLinkIndex) << (shiftForWord()))))),
            nilObj);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(aProcess)) && (!(isForwarded(aProcess))));
  assert(validStorePointerUncheckedArgs(MyListIndex, aProcess, nilObj));
  longAtput((void *)((aProcess + BaseHeaderSize) +
                     ((((usqInt)(MyListIndex) << (shiftForWord()))))),
            nilObj);
  return 1;
}

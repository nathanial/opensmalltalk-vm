/* Extracted from interp.c:49911 (function addLastLinktoList). */

/*	Add the given process to the end of the given linked list
        and set the backpointer of process to its new list. */

/* StackInterpreter>>#addLastLink:toList: */

static void addLastLinktoList(sqInt proc, sqInt aList) {
  sqInt lastLink;

  assert(!((isForwarded(proc))));
  assert(!((isForwarded(aList))));
  assert((fetchPointerofObject(NextLinkIndex, proc)) == (nilObject()));

  /* begin isEmptyList: */
  assert(!(isForwarded(aList)));
  if ((fetchPointerofObject(FirstLinkIndex, aList)) == nilObj) {
    storePointerofObjectwithValue(FirstLinkIndex, aList, proc);
  } else {
    lastLink = fetchPointerofObject(LastLinkIndex, aList);
    assert(lastLink != proc);

    storePointerofObjectwithValue(NextLinkIndex, lastLink, proc);
  }

  storePointerofObjectwithValue(LastLinkIndex, aList, proc);

  storePointerofObjectwithValue(MyListIndex, proc, aList);
}
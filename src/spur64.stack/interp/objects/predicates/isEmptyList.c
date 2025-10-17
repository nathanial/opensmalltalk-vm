/* Extracted from interp.c:54967 (function isEmptyList). */

/* StackInterpreter>>#isEmptyList: */

static int isEmptyList(sqInt aLinkedList) {
  assert(!(isForwarded(aLinkedList)));
  return (fetchPointerofObject(FirstLinkIndex, aLinkedList)) ==
         nilObj;
}
/* Extracted from interp.c:54967 (function isEmptyList). */

/* StackInterpreter>>#isEmptyList: */

static int isEmptyList(sqInt aLinkedList) {
  assert(!(isForwarded(aLinkedList)));
  return (longAt(
             (void *)((aLinkedList + BaseHeaderSize) +
                      ((((usqInt)(FirstLinkIndex) << (shiftForWord()))))))) ==
         nilObj;
}
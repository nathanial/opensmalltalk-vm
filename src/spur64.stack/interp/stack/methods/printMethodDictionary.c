/* Extracted from interp.c:60278 (function printMethodDictionary). */

/*	useful for VM debugging */

/* StackInterpreter>>#printMethodDictionary: */

void printMethodDictionary(sqInt dictionary) {
  sqInt index;
  sqInt meth;
  sqInt methodArray;
  sqInt selector;
  sqInt toDoLimit;

  methodArray =
      fetchPointerofObject(MethodArrayIndex, dictionary);
  toDoLimit = (numSlotsOf(dictionary)) - 1;
  for (index = SelectorStart; index <= toDoLimit; index += 1) {
    selector = fetchPointerofObject(index, dictionary);
    if (selector != nilObj) {
      meth = longAt((
          void *)((methodArray + BaseHeaderSize) +
                  ((((usqInt)((index - SelectorStart)) << (shiftForWord()))))));
      printOopShortInner(selector);
      print(" => ");
      printOopShortInner(meth);
      print(" (");
      printHex(selector);
      print(" => ");
      printHex(meth);
      print(")\n");
    }
  }
}
/* Extracted from interp.c:56444 (function lookupSelectorinClass). */

/*	Lookup selector in class. Answer the method or nil. This is a debugging
        routine. It does /not/ side-effect lookupClass or newMethod. */

/* StackInterpreter>>#lookupSelector:inClass: */

sqInt lookupSelectorinClass(sqInt selector, sqInt class) {
  sqInt currentClass;
  sqInt dictionary;
  usqInt index;
  usqInt length;
  sqInt mask;
  sqInt meth;
  sqInt methodArray;
  sqInt nextSelector;
  usqInt numSlots;
  sqInt objOop;
  sqInt objOopSqInt;
  sqInt wrapAround;

  currentClass = class;
  while (currentClass != nilObj) {
    dictionary = followObjFieldofObject(MethodDictionaryIndex, currentClass);
    if (dictionary == nilObj) {
      return null;
    }

    /* begin lookupMethodFor:InDictionary: */

    length = numSlotsOf(dictionary);
    mask = (length - SelectorStart) - 1;
    index = SelectorStart + (mask & ((((selector & (tagMask())) != 0)
                                          ? (selector >> 3)
                                          : (long32At((void *)(selector + 4))) &
                                                (identityHashHalfWordMask()))));

    /* It is assumed that there are some nils in this dictionary, and search
       will stop when one is encountered. However, if there are no nils, then
       wrapAround will be detected the second time the loop gets to the end of
       the table. */
    wrapAround = 0;
    while (1) {
      nextSelector = fetchPointerofObject(index, dictionary);
      if (nextSelector == nilObj) {
        meth = null;
        goto l1;
      }
      if (isOopForwarded(nextSelector)) {
        nextSelector = fixFollowedFieldofObjectwithInitialValue(
            index + SelectorStart, dictionary, nextSelector);
      }
      if (nextSelector == selector) {
        methodArray = followObjFieldofObject(MethodArrayIndex, dictionary);

        objOopSqInt = followFieldofObject(index - SelectorStart, methodArray);
        meth = objOopSqInt;
        goto l1;
      }
      index += 1;
      if (index == length) {
        if (wrapAround) {
          meth = null;
          goto l1;
        }
        wrapAround = 1;
        index = SelectorStart;
      }
    }
    meth = null;
    /* end lookupMethodFor:InDictionary: */
  l1:
    if (meth) {
      return meth;
    }

    /* begin superclassOf: */
    objOop = followObjFieldofObject(SuperclassIndex, currentClass);
    currentClass = objOop;
  }
  return null;
}
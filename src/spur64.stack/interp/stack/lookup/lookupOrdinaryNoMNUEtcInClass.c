/* Extracted from interp.c:56281 (function lookupOrdinaryNoMNUEtcInClass). */

/*	Lookup messageSelector in class. Answer 0 on success. Answer the splObj:
        index for the error selector to use on failure rather than performing
   MNU processing etc. */

/* StackInterpreter>>#lookupOrdinaryNoMNUEtcInClass: */

static sqInt lookupOrdinaryNoMNUEtcInClass(sqInt class) {
  sqInt currentClass;
  sqInt dictionary;
  sqInt found;
  sqInt index;
  usqInt length;
  sqInt mask;
  sqInt methodArray;
  sqInt nextSelector;
  usqInt numSlots;
  sqInt objOop;
  sqInt objOopSqInt;
  sqInt wrapAround;

  /* begin lookupBreakFor: */
  if ((breakSelectorLength <= 0) &&
      (/* shouldBreakForLookupIn:given: */
       (breakLookupClassTag) &&
       ((class == breakLookupClassTag) ||
        (((long32At((void *)(class + 4))) & (identityHashHalfWordMask())) ==
         breakLookupClassTag)))) {
    warning("lookup class send break (heartbeat suppressed)");
  }
  currentClass = class;
  while (currentClass != nilObj) {
    dictionary = followObjFieldofObject(MethodDictionaryIndex, currentClass);
    if (dictionary == nilObj) {
      /* begin superclassOf: */
      objOop = followObjFieldofObject(SuperclassIndex, currentClass);
      lkupClass = objOop;
      return SelectorCannotInterpret;
    }

    /* begin lookupMethodInDictionary: */

    length = numSlotsOf(dictionary);
    mask = (length - SelectorStart) - 1;

    /* Use linear search on small dictionaries; its cheaper.
       Also the limit can be set to force linear search of all dictionaries,
       which supports the booting of images that need rehashing (e.g. because a
       tracer has generated an image with different hashes but hasn't rehashed
       it yet.) */
    if (mask <= methodDictLinearSearchLimit) {
      index = 0;
      while (index <= mask) {
        nextSelector = fetchPointerofObject(index + SelectorStart, dictionary);
        if (isOopForwarded(nextSelector)) {
          nextSelector = fixFollowedFieldofObjectwithInitialValue(
              index + SelectorStart, dictionary, nextSelector);
        }
        if (nextSelector == messageSelector) {
          methodArray = followObjFieldofObject(MethodArrayIndex, dictionary);

          objOopSqInt = followFieldofObject(index, methodArray);
          newMethod = objOopSqInt;
          found = 1;
          goto l1;
        }
        index += 1;
      }
      found = 0;
      goto l1;
    }
    index = SelectorStart +
            (mask & ((((messageSelector & (tagMask())) != 0)
                          ? (messageSelector >> 3)
                          : (long32At((void *)(messageSelector + 4))) &
                                (identityHashHalfWordMask()))));

    /* It is assumed that there are some nils in this dictionary, and search
       will stop when one is encountered. However, if there are no nils, then
       wrapAround will be detected the second time the loop gets to the end of
       the table. */
    wrapAround = 0;
    while (1) {
      nextSelector = fetchPointerofObject(index, dictionary);
      if (nextSelector == nilObj) {
        found = 0;
        goto l1;
      }
      if (isOopForwarded(nextSelector)) {
        nextSelector = fixFollowedFieldofObjectwithInitialValue(
            index + SelectorStart, dictionary, nextSelector);
      }
      if (nextSelector == messageSelector) {
        methodArray = followObjFieldofObject(MethodArrayIndex, dictionary);

        objOopSqInt = followFieldofObject(index - SelectorStart, methodArray);
        newMethod = objOopSqInt;
        found = 1;
        goto l1;
      }
      index += 1;
      if (index == length) {
        if (wrapAround) {
          found = 0;
          goto l1;
        }
        wrapAround = 1;
        index = SelectorStart;
      }
    }
    found = 0;
    /* end lookupMethodInDictionary: */
  l1:
    if (found) {
      addNewMethodToCache(class);
      return 0;
    }

    /* begin superclassOf: */
    objOopSqInt = followObjFieldofObject(SuperclassIndex, currentClass);
    currentClass = objOopSqInt;
  }
  lkupClass = class;
  return SelectorDoesNotUnderstand;
}
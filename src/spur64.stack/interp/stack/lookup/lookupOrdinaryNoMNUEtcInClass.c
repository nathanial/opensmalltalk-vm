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
    /* begin followObjField:ofObject: */
    dictionary = fetchPointerofObject(MethodDictionaryIndex, currentClass);
    assert(isNonImmediate(dictionary));
    if ((!((longAt((void *)(dictionary))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      dictionary = fixFollowedFieldofObjectwithInitialValue(
          MethodDictionaryIndex, currentClass, dictionary);
    }
    if (dictionary == nilObj) {
      /* begin superclassOf: */
      /* begin followObjField:ofObject: */
      objOop = fetchPointerofObject(SuperclassIndex, currentClass);
      assert(isNonImmediate(objOop));
      if ((!((longAt((void *)(objOop))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex,
                                                          currentClass, objOop);
      }
      lkupClass = objOop;
      return SelectorCannotInterpret;
    }

    /* begin lookupMethodInDictionary: */
    /* begin numSlotsOf: */
    assert((classIndexOf(dictionary)) > (isForwardedObjectClassIndexPun()));
    length =
        (((numSlots =
               byteAt((void *)(dictionary + (numSlotsFieldByteOffset()))))) ==
                 (numSlotsMask())
             ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                        (void *)(dictionary - BaseHeaderSize))))
                                    << 8)))))) >>
                   8
             : numSlots);
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
          /* begin followObjField:ofObject: */
          methodArray = fetchPointerofObject(MethodArrayIndex, dictionary);
          assert(isNonImmediate(methodArray));
          if ((!((longAt((void *)(methodArray))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
            methodArray = fixFollowedFieldofObjectwithInitialValue(
                MethodArrayIndex, dictionary, methodArray);
          }

          /* begin followField:ofObject: */
          objOopSqInt = fetchPointerofObject(index, methodArray);
          if (isOopForwarded(objOopSqInt)) {
            objOopSqInt = fixFollowedFieldofObjectwithInitialValue(
                index, methodArray, objOopSqInt);
          }
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
        /* begin followObjField:ofObject: */
        methodArray = fetchPointerofObject(MethodArrayIndex, dictionary);
        assert(isNonImmediate(methodArray));
        if ((!((longAt((void *)(methodArray))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          methodArray = fixFollowedFieldofObjectwithInitialValue(
              MethodArrayIndex, dictionary, methodArray);
        }

        /* begin followField:ofObject: */
        objOopSqInt = fetchPointerofObject(index - SelectorStart, methodArray);
        if (isOopForwarded(objOopSqInt)) {
          objOopSqInt = fixFollowedFieldofObjectwithInitialValue(
              index - SelectorStart, methodArray, objOopSqInt);
        }
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
    /* begin followObjField:ofObject: */
    objOopSqInt = fetchPointerofObject(SuperclassIndex, currentClass);
    assert(isNonImmediate(objOopSqInt));
    if ((!((longAt((void *)(objOopSqInt))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      objOopSqInt = fixFollowedFieldofObjectwithInitialValue(
          SuperclassIndex, currentClass, objOopSqInt);
    }
    currentClass = objOopSqInt;
  }
  lkupClass = class;
  return SelectorDoesNotUnderstand;
}
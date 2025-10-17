/* Extracted from interp.c:53103 (function findClassContainingMethodstartingAt).
 */

/* StackInterpreter>>#findClassContainingMethod:startingAt: */

static sqInt findClassContainingMethodstartingAt(sqInt meth,
                                                               sqInt classObj) {
  sqInt classDict;
  usqInt classDictSize;
  sqInt currClass;
  sqInt i;
  sqInt methodArray;
  usqInt numSlots;
  sqInt objOop;
  sqInt referent;
  sqInt referentSqInt;

  if (/* isOopForwarded: */
      isOopForwarded(classObj)) {
    /* begin followForwarded: */
    assert(isUnambiguouslyForwarder(classObj));
    currClass = longAt(
        (void *)((classObj + BaseHeaderSize) + (0U << (shiftForWord()))));
    while (/* isOopForwarded: */
           isOopForwarded(currClass)) {
      currClass = longAt(
          (void *)((currClass + BaseHeaderSize) + (0U << (shiftForWord()))));
    }
  } else {
    currClass = classObj;
  }
  do {
    assert(!(isForwarded(currClass)));
    if (!(addressCouldBeClassObj(currClass))) {
      return nilObj;
    }

    /* begin noFixupFollowField:ofObject: */
    classDict = longAt(
        (void *)((currClass + BaseHeaderSize) +
                 ((((usqInt)(MethodDictionaryIndex) << (shiftForWord()))))));
    if (/* isOopForwarded: */
        isOopForwarded(classDict)) {
      /* begin followForwarded: */
      assert(isUnambiguouslyForwarder(classDict));
      referent = longAt(
          (void *)((classDict + BaseHeaderSize) + (0U << (shiftForWord()))));
      while (/* isOopForwarded: */
             isOopForwarded(referent)) {
        referent = longAt(
            (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
      }
      classDict = referent;
    }
    assert(!(isForwarded(classDict)));

    /* begin numSlotsOf: */
    assert((classIndexOf(classDict)) > (isForwardedObjectClassIndexPun()));
    classDictSize =
        (((numSlots =
               byteAt((void *)(classDict + (numSlotsFieldByteOffset()))))) ==
                 (numSlotsMask())
             ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                        (void *)(classDict - BaseHeaderSize))))
                                    << 8)))))) >>
                   8
             : numSlots);
    if (classDictSize > MethodArrayIndex) {
      /* begin noFixupFollowField:ofObject: */
      methodArray = longAt(
          (void *)((classDict + BaseHeaderSize) +
                   ((((usqInt)(MethodArrayIndex) << (shiftForWord()))))));
      if (/* isOopForwarded: */
          isOopForwarded(methodArray)) {
        /* begin followForwarded: */
        assert(isUnambiguouslyForwarder(methodArray));
        referentSqInt = longAt((void *)((methodArray + BaseHeaderSize) +
                                        (0U << (shiftForWord()))));
        while (/* isOopForwarded: */
               isOopForwarded(referentSqInt)) {
          referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) +
                                          (0U << (shiftForWord()))));
        }
        methodArray = referentSqInt;
      }
      assert(!(isForwarded(methodArray)));
      i = 0;
      while (i < (classDictSize - SelectorStart)) {
        if (meth == (noFixupFollowFieldofObject(i, methodArray))) {
          return currClass;
        }
        i += 1;
      }
    }

    /* begin noFixupSuperclassOf: */
    /* begin noFixupFollowField:ofObject: */
    objOop =
        longAt((void *)((currClass + BaseHeaderSize) +
                        ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
    if (/* isOopForwarded: */
        isOopForwarded(objOop)) {
      /* begin followForwarded: */
      assert(isUnambiguouslyForwarder(objOop));
      referentSqInt = longAt(
          (void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
      while (/* isOopForwarded: */
             isOopForwarded(referentSqInt)) {
        referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) +
                                        (0U << (shiftForWord()))));
      }
      objOop = referentSqInt;
    }
    currClass = objOop;
  } while (!(currClass == nilObj));
  return currClass;
}
/* Extracted from interp.c:53103 (function findClassContainingMethodstartingAt).
 */

/* StackInterpreter>>#findClassContainingMethod:startingAt: */

static sqInt findClassContainingMethodstartingAt(sqInt meth, sqInt classObj) {
  sqInt classDict;
  usqInt classDictSize;
  sqInt currClass;
  sqInt i;
  sqInt methodArray;
  usqInt numSlots;
  sqInt objOop;
  sqInt referent;
  sqInt referentSqInt;

  if (isOopForwarded(classObj)) {
    currClass = followForwarded(classObj);
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
    if (isOopForwarded(classDict)) {
      classDict = followForwarded(classDict);
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
      if (isOopForwarded(methodArray)) {
        methodArray = followForwarded(methodArray);
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
    if (isOopForwarded(objOop)) {
      objOop = followForwarded(objOop);
    }
    currClass = objOop;
  } while (!(currClass == nilObj));
  return currClass;
}
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
    classDict = fetchPointerofObject(MethodDictionaryIndex, currClass);
    if (isOopForwarded(classDict)) {
      classDict = followForwarded(classDict);
    }
    assert(!(isForwarded(classDict)));

    classDictSize = numSlotsOf(classDict);
    if (classDictSize > MethodArrayIndex) {
      /* begin noFixupFollowField:ofObject: */
      methodArray = fetchPointerofObject(MethodArrayIndex, classDict);
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
    objOop = fetchPointerofObject(SuperclassIndex, currClass);
    if (isOopForwarded(objOop)) {
      objOop = followForwarded(objOop);
    }
    currClass = objOop;
  } while (!(currClass == nilObj));
  return currClass;
}
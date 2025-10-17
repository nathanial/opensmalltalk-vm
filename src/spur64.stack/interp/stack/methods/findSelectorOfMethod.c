/* Extracted from interp.c:53459 (function findSelectorOfMethod). */

/* StackInterpreter>>#findSelectorOfMethod: */

sqInt findSelectorOfMethod(sqInt aMethodOop) {
  sqInt classDict;
  usqInt classDictSize;
  sqInt classObj;
  sqInt homeMethod;
  sqInt i;
  sqInt methodArray;
  usqInt numSlots;
  sqInt objOop;
  sqInt referent;
  sqInt selector;

  if (!(addressCouldBeObj(aMethodOop))) {
    return nilObj;
  }
  if ((!((longAt((void *)(aMethodOop))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    return findSelectorOfMethod(followForwarded(aMethodOop));
  }
  if (!(/* isOopCompiledMethod: */
        ((!(aMethodOop & (tagMask())))) &&
        (((byteAt((void *)(aMethodOop + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstCompiledMethodFormat())))) {
    return nilObj;
  }
  homeMethod = homeMethodOf(aMethodOop);
  if ((selector = maybeSelectorOfMethod(homeMethod))) {
    return selector;
  }
  classObj = safeMethodClassOf(homeMethod);
  if (addressCouldBeClassObj(classObj)) {
    /* begin noFixupFollowField:ofObject: */
    classDict = fetchPointerofObject(MethodDictionaryIndex, classObj);
    if (isOopForwarded(classDict)) {
      classDict = followForwarded(classDict);
    }

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
      methodArray = fetchPointerofObject(MethodArrayIndex, classDict);
      if (isOopForwarded(methodArray)) {
        methodArray = followForwarded(methodArray);
      }
      i = 0;
      while (i < (classDictSize - SelectorStart)) {
        if (homeMethod == (noFixupFollowFieldofObject(i, methodArray))) {
          /* begin noFixupFollowField:ofObject: */
          objOop = longAt((
              void *)((classDict + BaseHeaderSize) +
                      ((((usqInt)((i + SelectorStart)) << (shiftForWord()))))));
          if (isOopForwarded(objOop)) {
            objOop = followForwarded(objOop);
          }
          return objOop;
        }
        i += 1;
      }
    }
  }
  return nilObj;
}
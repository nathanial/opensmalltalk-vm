/* Extracted from interp.c:53103 (function findClassContainingMethodstartingAt).
 */

/* StackInterpreter>>#findClassContainingMethod:startingAt: */

static NoDbgRegParms sqInt findClassContainingMethodstartingAt(sqInt meth,
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
      ((!(classObj & (tagMask())))) &&
      ((!((longAt((void *)(classObj))) &
          ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
    /* begin followForwarded: */
    assert(isUnambiguouslyForwarder(classObj));
    currClass = longAt(
        (void *)((classObj + BaseHeaderSize) + (0U << (shiftForWord()))));
    while (/* isOopForwarded: */
           ((!(currClass & (tagMask())))) &&
           ((!((longAt((void *)(currClass))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
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
        ((!(classDict & (tagMask())))) &&
        ((!((longAt((void *)(classDict))) &
            ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
      /* begin followForwarded: */
      assert(isUnambiguouslyForwarder(classDict));
      referent = longAt(
          (void *)((classDict + BaseHeaderSize) + (0U << (shiftForWord()))));
      while (/* isOopForwarded: */
             ((!(referent & (tagMask())))) &&
             ((!((longAt((void *)(referent))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
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
          ((!(methodArray & (tagMask())))) &&
          ((!((longAt((void *)(methodArray))) &
              ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
        /* begin followForwarded: */
        assert(isUnambiguouslyForwarder(methodArray));
        referentSqInt = longAt((void *)((methodArray + BaseHeaderSize) +
                                        (0U << (shiftForWord()))));
        while (/* isOopForwarded: */
               ((!(referentSqInt & (tagMask())))) &&
               ((!((longAt((void *)(referentSqInt))) &
                   ((classIndexMask()) -
                    (isForwardedObjectClassIndexPun())))))) {
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
        ((!(objOop & (tagMask())))) &&
        ((!((longAt((void *)(objOop))) &
            ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
      /* begin followForwarded: */
      assert(isUnambiguouslyForwarder(objOop));
      referentSqInt = longAt(
          (void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
      while (/* isOopForwarded: */
             ((!(referentSqInt & (tagMask())))) &&
             ((!((longAt((void *)(referentSqInt))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
        referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) +
                                        (0U << (shiftForWord()))));
      }
      objOop = referentSqInt;
    }
    currClass = objOop;
  } while (!(currClass == nilObj));
  return currClass;
}
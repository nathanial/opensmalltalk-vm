/* Extracted from interp.c:39309 (function markAndTraceClassOf). */

/*	Ensure the class of the argument is marked, pushing it on the markStack
   if not already marked. And for one-way become, which can create duplicate
   entries in the class table, make sure objOop's classIndex refers to the
   classObj's actual classIndex. Note that this is recursive, but the metaclass
   chain should terminate quickly.
 */

/* SpurMemoryManager>>#markAndTraceClassOf: */

static void markAndTraceClassOf(sqInt objOop) {
  sqInt classIndex;
  sqInt classObj;
  sqInt classTablePage;
  sqInt fieldIndex;
  sqInt objStack;
  sqInt realClassIndex;

  classIndex = (longAt((void *)(objOop))) & (classIndexMask());

  /* begin classOrNilAtIndex: */
  assert((classIndex <= (tagMask())) || (classIndex >= (arrayClassIndexPun())));
  fieldIndex = ((usqInt)(classIndex)) >> (classTableMajorIndexShift());

  classTablePage =
      fetchPointerofObject(fieldIndex, hiddenRootsObj);
  if (classTablePage == nilObj) {
    classObj = nilObj;
    goto l1;
  }
  fieldIndex = classIndex & ((1U << (classTableMajorIndexShift())) - 1);
  classObj = fetchPointerofObject(fieldIndex, classTablePage);
  /* end classOrNilAtIndex: */
l1:
  assert(objCouldBeClassObj(classObj));
  realClassIndex =
      (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask());
  if ((classIndex != realClassIndex) && (classIndex > (lastClassIndexPun()))) {
    /* begin setClassIndexOf:to: */
    assert(((realClassIndex >= 0) && (realClassIndex <= (classIndexMask()))));
    longAtput((void *)(objOop), ((longAt((void *)(objOop))) &
                                 (~(usqIntptr_t)(classIndexMask()))) +
                                    realClassIndex);
  }
  if (!((byteAt((void *)(classObj + (markBitsByteOffset())))) &
        (1U << (markedBitByteShift())))) {
    /* begin setIsMarkedOf:to: */
    assert(!(isFreeObject(classObj)));
    byteAtput((void *)(classObj + (markBitsByteOffset())),
              (byteAt((void *)(classObj + (markBitsByteOffset())))) |
                  (1U << (markedBitByteShift())));
    markAndTraceClassOf(classObj);
    objStack = markStack;

    /* begin push:onObjStack: */
    assert(addressCouldBeOop(classObj));
    if (((classObj & (tagMask())) != 0)) {
      assert(objStack == markStack);
      assert(addressCouldBeObj(
          topOfObjStack((0 == (fetchPointerofObject(ObjStackTopx, objStack))
                             ? fetchPointerofObject(ObjStackNextx, objStack)
                             : objStack))));
    } else {
      assert(!((objStack == markStack) && (isWeakNonImm(classObj))));

      /* There should only be weaklings on the weaklingStack */
      assert((objStack != weaklingStack) || (isWeakNonImm(classObj)));
    }

    /* There should be no weaklings on the mark stack. */
    noCheckPushonObjStack(classObj, objStack);
  }
}
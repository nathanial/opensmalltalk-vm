/* Extracted from interp.c:19686 (function primitiveNextInstance). */

/* InterpreterPrimitives>>#primitiveNextInstance */

static void primitiveNextInstance(void) {
  sqInt actualObj;
  sqInt classIndex;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt object;
  sqInt subsequentObject;

  object = longAt(stackPointer);
  if (!(((object & (tagMask())) != 0))) {
    /* begin instanceAfter: */
    actualObj = object;
    classIndex = (longAt((void *)(object))) & (classIndexMask());

    /* begin isYoungObject: */
    assert(isNonImmediate(object));
    if (oopisLessThan(object, oldSpaceStart)) {
      if (oopisGreaterThanOrEqualToandLessThan(object, ((eden).start),
                                               freeStart)) {
        while (1) {
          /* begin objectAfter:limit: */
          followingWordAddress = addressAfter(actualObj);
          if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
            actualObj = freeStart;
            goto l1;
          }
          followingWord = longAt((void *)(followingWordAddress));
          actualObj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                               (numSlotsMask())
                           ? followingWordAddress + BaseHeaderSize
                           : followingWordAddress);
          /* end objectAfter:limit: */
        l1:;
          if (!(oopisLessThan(actualObj, freeStart)))
            break;
          if (classIndex ==
              ((longAt((void *)(actualObj))) & (classIndexMask()))) {
            subsequentObject = actualObj;
            goto l4;
          }
        }
        actualObj = (oopisGreaterThan(pastSpaceStart, ((pastSpace).start))
                         ? objectStartingAt(((pastSpace).start))
                         : nilObj);
      }
      if (oopisGreaterThanOrEqualToandLessThan(actualObj, ((pastSpace).start),
                                               pastSpaceStart)) {
        while (1) {
          /* begin objectAfter:limit: */
          followingWordAddress = addressAfter(actualObj);
          if (oopisGreaterThanOrEqualTo(followingWordAddress, pastSpaceStart)) {
            actualObj = pastSpaceStart;
            goto l2;
          }
          followingWord = longAt((void *)(followingWordAddress));
          actualObj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                               (numSlotsMask())
                           ? followingWordAddress + BaseHeaderSize
                           : followingWordAddress);
          /* end objectAfter:limit: */
        l2:;
          if (!(oopisLessThan(actualObj, pastSpaceStart)))
            break;
          if (classIndex ==
              ((longAt((void *)(actualObj))) & (classIndexMask()))) {
            subsequentObject = actualObj;
            goto l4;
          }
        }
        actualObj = nilObj;
      }
    }

    /* it almost never is because usually this is preceded by a send of
     * initialInstanceOf: */
    while (1) {
      /* begin objectAfter:limit: */
      followingWordAddress = addressAfter(actualObj);
      if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
        actualObj = endOfMemory;
        goto l3;
      }
      followingWord = longAt((void *)(followingWordAddress));
      actualObj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                           (numSlotsMask())
                       ? followingWordAddress + BaseHeaderSize
                       : followingWordAddress);
      /* end objectAfter:limit: */
    l3:;
      if (!(oopisLessThan(actualObj, endOfMemory)))
        break;
      if (classIndex == ((longAt((void *)(actualObj))) & (classIndexMask()))) {
        subsequentObject = actualObj;
        goto l4;
      }
    }
    subsequentObject = null;
    /* end instanceAfter: */
  l4:
    if (subsequentObject) {
      /* begin pop:thenPush: */
      popthenPush(argumentCount + 1, subsequentObject);
      return;
    }
  }

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
}
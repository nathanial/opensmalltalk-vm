/* Extracted from interp.c:43750 (function printMethodReferencesTo). */

/*	Scan the heap printing the oops of any and all methods that refer to
 * anOop */
/*	useful for VM debugging */

/* SpurMemoryManager>>#printMethodReferencesTo: */

void printMethodReferencesTo(sqInt anOop) {
  sqInt address;
  sqInt classIndex;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt i;
  usqInt numSlots;
  sqInt objSqInt;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt startObject;

  /* begin allObjectsDo: */
  address = /* startAddressForBridgedHeapEnumeration */
      (pastSpaceStart > (((pastSpace).start))
           ? ((pastSpace).start)
           : (freeStart > (((eden).start)) ? ((eden).start) : oldSpaceStart));

  /* begin objectStartingAt: */
  numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
  startObject =
      (numSlots == (numSlotsMask()) ? address + BaseHeaderSize : address);

  /* begin allEntitiesFrom:do: */
  prevPrevObj = (prevObj = null);
  objSqInt = startObject;
  enableObjectEnumerationFrom(startObject);
  while (1) {
    assert((objSqInt % (allocationUnit())) == 0);
    if (!(oopisLessThan(objSqInt, endOfMemory)))
      break;
    assert((long64At((void *)(objSqInt))) != 0);

    /* begin isEnumerableObject: */
    classIndex = (longAt((void *)(objSqInt))) & (classIndexMask());
    assert((classIndex == (segmentBridgePun())) ||
           ((classIndex == (isForwardedObjectClassIndexPun())) ||
            (((long64At((void *)(objSqInt))) != 0) &&
             (classIndex < (numClassTablePages * (classTablePageSize()))))));
    if (classIndex >= (isForwardedObjectClassIndexPun())) {
      if (((byteAt((void *)(objSqInt + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstCompiledMethodFormat())) {
        i = ((literalCountOf(objSqInt)) + LiteralStart) - 1;
        while (((i -= 1)) >= 0) {
          if (anOop ==
              (longAt((void *)((objSqInt + BaseHeaderSize) +
                               ((((usqInt)(i) << (shiftForWord())))))))) {
            printHex(objSqInt);
            print(" @ ");
            printNum(i);

            /* begin space */
            printChar(' ');
            printOopShortInner(objSqInt);
            cr();
            i = 0;
          }
        }
      }
    }
    prevPrevObj = prevObj;
    prevObj = objSqInt;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(objSqInt);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objSqInt = endOfMemory;
      goto l1;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objSqInt =
        ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                 (numSlotsMask())
             ? ((oopisLessThan(objSqInt, oldSpaceStart)) &&
                        ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
                    ? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
                    : followingWordAddress + BaseHeaderSize)
             : followingWordAddress);
    /* end objectAfterMaybeSlimBridge:limit: */
  l1:
    assert(oopisGreaterThan(objSqInt, prevObj));
  }
}
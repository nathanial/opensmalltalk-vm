/* Extracted from interp.c:44486 (function printReferencesTo). */

/*	Scan the heap printing the oops of any and all objects that refer to
 * anOop */
/*	useful for VM debugging */

/* SpurMemoryManager>>#printReferencesTo: */

void printReferencesTo(sqInt anOop) {
  sqInt address;
  sqInt classIndex;
  sqInt contextSize;
  sqInt fmt;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt header;
  sqInt i;
  usqInt numLiterals;
  usqInt numSlots;
  sqInt objSqInt;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt sp;
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
      /* begin numPointerSlotsOf: */
      fmt = (byteAt((void *)(objSqInt + (formatFieldByteOffset())))) &
            (formatMask());
      if (fmt <= 5 /* lastPointerFormat */) {
        if ((fmt == (indexablePointersFormat())) &&
            (((longAt((void *)(objSqInt))) & (classIndexMask())) ==
             ClassMethodContextCompactIndex)) {
          /* begin fetchStackPointerOf: */
          sp = fetchPointerofObject(StackPointerIndex, objSqInt);
          if (!((((sp) & 7) == 1))) {
            contextSize = 0;
            goto l2;
          }
          assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(objSqInt)));
          contextSize = (sp >> 3);
          /* end fetchStackPointerOf: */
        l2:
          i = ((usqInt)(CtxtTempFrameStart + contextSize));
          goto l3;
        }

        /* contexts end at the stack pointer */

        i = numSlotsOf(objSqInt);
        goto l3;
      }
      if (fmt == (forwardedFormat())) {
        i = 1;
        goto l3;
      }
      if (fmt < (firstCompiledMethodFormat())) {
        i = 0;
        goto l3;
      }

      header = methodHeaderOf(objSqInt);

      /* begin literalCountOfMethodHeader: */
      assert((((header) & 7) == 1));
      numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
      i = numLiterals + LiteralStart;
      /* end numPointerSlotsOf: */
    l3:
      while (((i -= 1)) >= 0) {
        if (anOop == (fetchPointerofObject(i, objSqInt))) {
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
  if (isonObjStack(anOop, markStack)) {
    print("on markStack");
    cr();
  }
  if (isonObjStack(anOop, weaklingStack)) {
    print("on weaklingStack");
    cr();
  }
  if (isonObjStack(anOop, mournQueue)) {
    print("in mournQueue");
    cr();
  }
  if ((((byteAt((void *)(anOop + (formatFieldByteOffset())))) &
        (1U << (rememberedBitByteShift()))) != 0) &&
      (isInRememberedSet(anOop))) {
    print("in rememberedSet");
    cr();
  }
}
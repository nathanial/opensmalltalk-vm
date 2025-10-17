/* Extracted from interp.c:32289 (function
 * checkHeapIntegrityclassIndicesShouldBeValid). */

/*	Perform an integrity/leak check using the heapMap. Assume
        clearLeakMapAndMapAccessibleObjects has set a bit at each (non-free)
        object's header. Scan all objects in the heap checking that every
        pointer points to a header. Scan the rememberedSet, remapBuffer and
        extraRootTable checking
        that every entry is a pointer to a header. Check that the number of
   roots is correct and that all rememberedSet entries have their isRemembered:
   flag set. Answer if all checks pass. */

/* SpurMemoryManager>>#checkHeapIntegrity:classIndicesShouldBeValid: */

static sqInt
checkHeapIntegrityclassIndicesShouldBeValid(sqInt excludeUnmarkedObjs,
                                            sqInt classIndicesShouldBeValid) {
  sqInt chunkBytes;
  sqInt classIndex;
  sqInt classIndexSqInt;
  sqInt classOop;
  sqInt classTablePage;
  sqInt containsYoung;
  sqInt fi;
  sqInt fieldIndex;
  sqInt fieldOop;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt freeListOop;
  sqInt i;
  sqInt iSqInt;
  sqInt numOnThisPage;
  sqInt numRememberedObjectsInHeap;
  usqInt numSlots;
  sqInt obj;
  sqInt objOop;
  sqInt objStackPage;
  sqInt ok;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt ri;
  usqInt start;
  sqInt toDoLimit;
  sqInt toDoLimit1;

  /* Almost all of the time spent here used to go into the asserts in
     fetchPointer:ofObject: in the simulator class overrides. Since we know here
     the indices used are valid we temporarily remove them to claw back that
     performance. */
  ok = 1;
  numRememberedObjectsInHeap = 0;
  for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
    if ((freeListOop = freeLists[i])) {
      if (oopisGreaterThanOrEqualToandLessThan(freeListOop, oldSpaceStart,
                                               endOfMemory)) {
        if (heapMapAtWord(pointerForOop(freeListOop))) {
          fprintf(transcript, "leak in free list %d to non-free %p\n", ((int)i),
                  ((void *)freeListOop));
          ok = 0;
        }
      } else {
        fprintf(transcript, "leak in free list %d to out-of-bounds %p\n",
                ((int)i), ((void *)freeListOop));
        ok = 0;
      }
    }
  }

  /* Excuse the duplication but performance is at a premium and we avoid
     some tests by splitting the newSpace and oldSpace enumerations. */

  /* begin allNewSpaceEntitiesDo: */
  prevPrevObj = (prevObj = null);

  /* After a scavenge eden is empty, futureSpace is empty, and all newSpace
     objects are in pastSpace.  Objects are allocated in eden.  So enumerate
     only pastSpace and eden. */
  assert((((pastSpace).start)) < (((eden).start)));
  start = /* startAddressForBridgedHeapEnumeration */
      (pastSpaceStart > (((pastSpace).start))
           ? ((pastSpace).start)
           : (freeStart > (((eden).start)) ? ((eden).start) : oldSpaceStart));
  if (start > freeStart) {
    goto l6;
  }

  /* begin bridgePastSpaceAndEden */
  if (pastSpaceStart < (((eden).start))) {
    if ((pastSpaceStart + BaseHeaderSize) == (((eden).start))) {
      hackSlimBridgeToat(objectStartingAt(((eden).start)), pastSpaceStart);

      /* And carefully check the assumption */
      assert((objectAfterMaybeSlimBridgelimit(
                 objectInPastSpaceBefore(pastSpaceStart), nilObj)) ==
             (objectStartingAt(((eden).start))));
    } else {
      initSegmentBridgeWithBytesat((((eden).start)) - pastSpaceStart,
                                   pastSpaceStart);
    }
  }

  /* begin objectStartingAt: */
  numSlots = byteAt((void *)(start + (numSlotsFieldByteOffset())));
  objOop = (numSlots == (numSlotsMask()) ? start + BaseHeaderSize : start);
  while (oopisLessThan(objOop, freeStart)) {
    if (((longAt((void *)(objOop))) & (classIndexMask())) ==
        (isFreeObjectClassIndexPun())) {
      fprintf(transcript, "young object %p is free\n", ((void *)objOop));
      ok = 0;
    } else {
      if (!((!(((byteAt((void *)(objOop + (markBitsByteOffset())))) &
                (1U << (markedBitByteShift()))) != 0)) &&
            (excludeUnmarkedObjs))) {
        if ((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift()))) {
          fprintf(transcript, "young object %p is remembered\n",
                  ((void *)objOop));
          ok = 0;
        }
      }
      if ((!((longAt((void *)(objOop))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        fieldOop = longAt(
            (void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
        if (/* isInHeapBounds: */
            (oopisGreaterThanOrEqualTo(fieldOop, newSpaceStart)) &&
            (oopisLessThan(fieldOop, endOfMemory))) {
          if (!(heapMapAtWord(pointerForOop(fieldOop)))) {
            fprintf(transcript, "object leak in forwarder %p to unmapped %p\n",
                    ((void *)objOop), ((void *)fieldOop));
            ok = 0;
          }
        } else {
          fprintf(transcript,
                  "object leak in forwarder %p to out-of-bounds %p\n",
                  ((void *)objOop), ((void *)fieldOop));
          ok = 0;
        }
      } else {
        classIndexSqInt =
            (classIndex = (longAt((void *)(objOop))) & (classIndexMask()));

        /* begin classOrNilAtIndex: */
        assert((classIndexSqInt <= (tagMask())) ||
               (classIndexSqInt >= (arrayClassIndexPun())));
        fieldIndex =
            ((usqInt)(classIndexSqInt)) >> (classTableMajorIndexShift());

        /* begin fetchPointer:ofObject: */
        classTablePage =
            longAt((void *)((hiddenRootsObj + BaseHeaderSize) +
                            ((((usqInt)(fieldIndex) << (shiftForWord()))))));
        if (classTablePage == nilObj) {
          classOop = nilObj;
          goto l1;
        }
        fieldIndex =
            classIndexSqInt & ((1U << (classTableMajorIndexShift())) - 1);
        classOop =
            longAt((void *)((classTablePage + BaseHeaderSize) +
                            ((((usqInt)(fieldIndex) << (shiftForWord()))))));
        /* end classOrNilAtIndex: */
      l1:
        if (classIndicesShouldBeValid &&
            ((classOop == nilObj) &&
             (!(((longAt((void *)(objOop))) & (classIndexMask())) <=
                (lastClassIndexPun()))))) {
          fprintf(transcript,
                  "object leak in %p invalid class index %p -> nilObj\n",
                  ((void *)objOop), ((void *)classIndex));
          ok = 0;
        }
        toDoLimit = (numPointerSlotsOf(objOop)) - 1;
        for (fi = 0; fi <= toDoLimit; fi += 1) {
          fieldOop = longAt((void *)((objOop + BaseHeaderSize) +
                                     ((((usqInt)(fi) << (shiftForWord()))))));
          if ((!(fieldOop & (tagMask())))) {
            if (/* isInHeapBounds: */
                (oopisGreaterThanOrEqualTo(fieldOop, newSpaceStart)) &&
                (oopisLessThan(fieldOop, endOfMemory))) {
              if (!(heapMapAtWord(pointerForOop(fieldOop)))) {
                fprintf(transcript, "object leak in %p @ %d = %p\n",
                        ((void *)objOop), ((int)fi), ((void *)fieldOop));
                ok = 0;
              }
            } else {
              fprintf(transcript, "object leak in %p to out-of-bounds %p\n",
                      ((void *)objOop), ((void *)fieldOop));
              ok = 0;
            }
          }
        }
      }
    }
    prevPrevObj = prevObj;
    prevObj = objOop;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(objOop);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
      objOop = freeStart;
      goto l5;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOop =
        ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                 (numSlotsMask())
             ? ((oopisLessThan(objOop, oldSpaceStart)) &&
                        ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
                    ? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
                    : followingWordAddress + BaseHeaderSize)
             : followingWordAddress);
    /* end objectAfterMaybeSlimBridge:limit: */
  l5:;
  }
  /* end allNewSpaceEntitiesDo: */
l6:

  /* begin allOldSpaceEntitiesDo: */
  /* begin allOldSpaceEntitiesFrom:do: */
  assert(isOldObject(nilObj));
  prevPrevObj = (prevObj = null);
  objOop = nilObj;
  while (1) {
    assert((objOop % (allocationUnit())) == 0);
    if (!(oopisLessThan(objOop, endOfMemory)))
      break;
    assert((long64At((void *)(objOop))) != 0);
    if (((longAt((void *)(objOop))) & (classIndexMask())) ==
        (isFreeObjectClassIndexPun())) {
      if (heapMapAtWord(pointerForOop(objOop))) {
        fprintf(transcript, "leak in free chunk %p is mapped?! \n",
                ((void *)objOop));
        ok = 0;
      }
      fieldOop = longAt(
          (void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
      if ((fieldOop != 0) && ((heapMapAtWord(pointerForOop(fieldOop))) != 0)) {
        fprintf(transcript, "leak in free chunk %p @ 0 = %p is mapped\n",
                ((void *)objOop), ((void *)fieldOop));
        ok = 0;
      }
      chunkBytes = bytesInBody(objOop);

      /* begin isLilliputianSize: */
      assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
      if (!(chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */))) {
        fieldOop = longAt(
            (void *)((objOop + BaseHeaderSize) + (1U << (shiftForWord()))));
        if ((fieldOop != 0) &&
            ((heapMapAtWord(pointerForOop(fieldOop))) != 0)) {
          fprintf(transcript, "leak in free chunk %p @ 1 = %p is mapped\n",
                  ((void *)objOop), ((void *)fieldOop));
          ok = 0;
        }
      }
      if ((bytesInBody(objOop)) >=
          0x200 /* (numFreeLists * #allocationUnit) */) {
        for (fi = 2 /* freeChunkParentIndex */;
             fi <= 4 /* freeChunkLargerIndex */; fi += 1) {
          fieldOop = longAt((void *)((objOop + BaseHeaderSize) +
                                     ((((usqInt)(fi) << (shiftForWord()))))));
          if ((fieldOop != 0) &&
              ((heapMapAtWord(pointerForOop(fieldOop))) != 0)) {
            fprintf(transcript, "leak in free chunk %p @ %d = %p is mapped\n",
                    ((void *)objOop), ((int)fi), ((void *)fieldOop));
            ok = 0;
          }
        }
      }
    } else {
      if (!(excludeUnmarkedObjs &&
            (!(((byteAt((void *)(objOop + (markBitsByteOffset())))) &
                (1U << (markedBitByteShift()))) != 0)))) {
        containsYoung = 0;
        if ((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift()))) {
          numRememberedObjectsInHeap += 1;

          /* begin isInRememberedSet: */
          for (iSqInt = 0; iSqInt < rememberedSetSize; iSqInt += 1) {
            if ((rememberedSet[iSqInt]) == objOop) {
              goto l2;
            }
          }
          fprintf(transcript,
                  "remembered object %p is not in remembered table\n",
                  ((void *)objOop));
          ok = 0;
        l2:;
        }
        if ((!((longAt((void *)(objOop))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          fieldOop = longAt(
              (void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
          if (/* isInHeapBounds: */
              (oopisGreaterThanOrEqualTo(fieldOop, newSpaceStart)) &&
              (oopisLessThan(fieldOop, endOfMemory))) {
            if (!(heapMapAtWord(pointerForOop(fieldOop)))) {
              fprintf(transcript,
                      "object leak in forwarder %p to unmapped %p\n",
                      ((void *)objOop), ((void *)fieldOop));
              ok = 0;
            }
          } else {
            fprintf(transcript,
                    "object leak in forwarder %p to out-of-bounds %p\n",
                    ((void *)objOop), ((void *)fieldOop));
            ok = 0;
          }
          if (/* isReallyYoung: */
              ((!(fieldOop & (tagMask())))) &&
              ((/* begin isReallyYoungObject: */ /* begin isYoungObject: */
                assert(isNonImmediate(fieldOop)),
                oopisLessThan(fieldOop, oldSpaceStart)))) {
            containsYoung = 1;
          }
        } else {
          classIndexSqInt =
              (classIndex = (longAt((void *)(objOop))) & (classIndexMask()));

          /* begin classOrNilAtIndex: */
          assert((classIndexSqInt <= (tagMask())) ||
                 (classIndexSqInt >= (arrayClassIndexPun())));
          fieldIndex =
              ((usqInt)(classIndexSqInt)) >> (classTableMajorIndexShift());

          /* begin fetchPointer:ofObject: */
          classTablePage =
              longAt((void *)((hiddenRootsObj + BaseHeaderSize) +
                              ((((usqInt)(fieldIndex) << (shiftForWord()))))));
          if (classTablePage == nilObj) {
            classOop = nilObj;
            goto l3;
          }
          fieldIndex =
              classIndexSqInt & ((1U << (classTableMajorIndexShift())) - 1);
          classOop =
              longAt((void *)((classTablePage + BaseHeaderSize) +
                              ((((usqInt)(fieldIndex) << (shiftForWord()))))));
          /* end classOrNilAtIndex: */
        l3:
          if (classIndicesShouldBeValid &&
              ((classOop == nilObj) && (classIndex > (lastClassIndexPun())))) {
            fprintf(transcript,
                    "object leak in %p invalid class index %p -> nilObj\n",
                    ((void *)objOop), ((void *)classIndex));
            ok = 0;
          }
          toDoLimit1 = (numPointerSlotsOf(objOop)) - 1;
          for (fi = 0; fi <= toDoLimit1; fi += 1) {
            fieldOop = longAt((void *)((objOop + BaseHeaderSize) +
                                       ((((usqInt)(fi) << (shiftForWord()))))));
            if ((!(fieldOop & (tagMask())))) {
              if (/* isInHeapBounds: */
                  (oopisGreaterThanOrEqualTo(fieldOop, newSpaceStart)) &&
                  (oopisLessThan(fieldOop, endOfMemory))) {
                if (!(heapMapAtWord(pointerForOop(fieldOop)))) {
                  fprintf(transcript, "object leak in %p @ %d = %p\n",
                          ((void *)objOop), ((int)fi), ((void *)fieldOop));
                  ok = 0;
                }
              } else {
                fprintf(transcript, "object leak in %p to out-of-bounds %p\n",
                        ((void *)objOop), ((void *)fieldOop));
                ok = 0;
              }

              /* don't be misled by CogMethods; they appear to be young, but
               * they're not */
              if (/* isReallyYoung: */
                  ((!(fieldOop & (tagMask())))) &&
                  ((/* begin isReallyYoungObject: */ /* begin isYoungObject: */
                    assert(isNonImmediate(fieldOop)),
                    oopisLessThan(fieldOop, oldSpaceStart)))) {
                containsYoung = 1;
              }
            }
          }
        }
        if (containsYoung) {
          if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            fprintf(transcript,
                    "unremembered object %p contains young oop(s)\n",
                    ((void *)objOop));
            ok = 0;
          }
        }
      }
    }
    prevPrevObj = prevObj;
    prevObj = objOop;

    /* begin objectAfter:limit: */
    followingWordAddress = addressAfter(objOop);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objOop = endOfMemory;
      goto l7;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                      (numSlotsMask())
                  ? followingWordAddress + BaseHeaderSize
                  : followingWordAddress);
    /* end objectAfter:limit: */
  l7:;
  }
  if (numRememberedObjectsInHeap != rememberedSetSize) {
    fprintf(transcript, "root count mismatch. #heap roots %d; #roots %d\n",
            ((int)numRememberedObjectsInHeap), ((int)rememberedSetSize));

    /* But the system copes with overflow... */
  }

  /* begin rememberedSetWithIndexDo: */
  for (iSqInt = 0; iSqInt < rememberedSetSize; iSqInt += 1) {
    obj = rememberedSet[iSqInt];
    if (obj & (BytesPerWord - 1)) {
      fprintf(transcript, "misaligned oop in remembered set @ %d = %p\n",
              ((int)iSqInt), ((void *)obj));
      ok = 0;
    } else {
      if (heapMapAtWord(pointerForOop(obj))) {
        if (/* isYoung: */
            ((!(obj & (tagMask())))) && (oopisLessThan(obj, oldSpaceStart))) {
          fprintf(transcript, "non-root in remembered set @ %d = %p\n",
                  ((int)iSqInt), ((void *)obj));
          ok = 0;
        }
      } else {
        fprintf(transcript, "object leak in remembered set @ %d = %p\n",
                ((int)iSqInt), ((void *)obj));
        ok = 0;
      }
    }
  }

  /* begin objStack:do: */
  if (mournQueue == nilObj) {
    goto l4;
  }
  eassert(isValidObjStack(mournQueue));
  objStackPage = mournQueue;
  while (objStackPage != 0) {
    numOnThisPage =
        longAt((void *)((objStackPage + BaseHeaderSize) +
                        ((((usqInt)(ObjStackTopx) << (shiftForWord()))))));
    for (iSqInt = ((numOnThisPage + ObjStackFixedSlots) - 1);
         iSqInt >= ObjStackFixedSlots; iSqInt += -1) {
      obj = longAt((void *)((objStackPage + BaseHeaderSize) +
                            ((((usqInt)(iSqInt) << (shiftForWord()))))));
      if (obj & (BytesPerWord - 1)) {
        fprintf(transcript, "misaligned oop in mournQueue @ %d in %p = %p\n",
                ((int)iSqInt), ((void *)objStackPage), ((void *)obj));
        ok = 0;
      } else {
        if (!(excludeUnmarkedObjs &&
              (!(((byteAt((void *)(obj + (markBitsByteOffset())))) &
                  (1U << (markedBitByteShift()))) != 0)))) {
          if (!(heapMapAtWord(pointerForOop(obj)))) {
            fprintf(transcript, "object leak in mournQueue @ %d in %p = %p\n",
                    ((int)iSqInt), ((void *)objStackPage), ((void *)obj));
            ok = 0;
          }
        }
      }
    }
    objStackPage =
        longAt((void *)((objStackPage + BaseHeaderSize) +
                        ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
  }
  /* end objStack:do: */
l4:
  for (ri = 1; ri <= remapBufferCount; ri += 1) {
    obj = remapBuffer[ri];
    if (obj & (BytesPerWord - 1)) {
      fprintf(transcript, "misaligned remapRoot @ %d = %p\n", ((int)ri),
              ((void *)obj));
      ok = 0;
    } else {
      if (!(heapMapAtWord(pointerForOop(obj)))) {
        fprintf(transcript, "object leak in remapRoots @ %d = %p\n", ((int)ri),
                ((void *)obj));
        ok = 0;
      }
    }
  }
  for (ri = 1; ri <= extraRootCount; ri += 1) {
    obj = (extraRoots[ri])[0];
    if (obj & (BytesPerWord - 1)) {
      fprintf(transcript, "misaligned extraRoot @ %d => %p\n", ((int)ri),
              ((void *)obj));
      ok = 0;
    } else {
      if (!(heapMapAtWord(pointerForOop(obj)))) {
        fprintf(transcript, "object leak in extraRoots @ %d => %p\n", ((int)ri),
                ((void *)obj));
        ok = 0;
      }
    }
  }
  return ok;
}
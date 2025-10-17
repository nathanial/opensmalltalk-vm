/* Extracted from interp.c:31274 (function becomewithtwoWaycopyHash). */

/*	All references to each object in array1 are swapped with all references
   to the corresponding object in array2. That is, all pointers to one object
        are replaced
        with with pointers to the other. The arguments must be arrays of the
   same length. Answers PrimNoErr if the primitive succeeds, otherwise a
   relevant error code.
 */
/*	Implementation: Uses lazy forwarding to defer updating references until
        message send.
 */

/* SpurMemoryManager>>#become:with:twoWay:copyHash: */

static sqInt becomewithtwoWaycopyHash(sqInt array1, sqInt array2,
                                      sqInt twoWayFlag, sqInt copyHashFlag) {
  sqInt contextSize;
  sqInt ec;
  sqInt fieldOffset;
  sqInt fmt;
  sqInt hash;
  sqInt header;
  sqInt i;
  sqInt newObj1;
  sqInt newObj2;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt o1ClassIndex;
  sqInt o2ClassIndex;
  sqInt obj1;
  sqInt obj2;
  sqInt objOop;
  sqInt objOopSqInt;
  sqInt oop1;
  sqInt oop2;
  sqInt referent;
  sqInt referentSqInt;
  sqInt size;
  sqInt sp;
  sqInt toDoLimit;

  assert(becomeEffectsFlags == 0);
  runLeakCheckerFor(GCModeBecome);
  if (!(/* isArray: */
        ((!(array1 & (tagMask())))) &&
        (((byteAt((void *)(array1 + (formatFieldByteOffset())))) &
          (formatMask())) == (arrayFormat())))) {
    return PrimErrBadReceiver;
  }
  if (!((/* isArray: */
         ((!(array2 & (tagMask())))) &&
         (((byteAt((void *)(array2 + (formatFieldByteOffset())))) &
           (formatMask())) == (arrayFormat()))) &&
        (((/* begin numSlotsOf: */
           assert((classIndexOf(array1)) > (isForwardedObjectClassIndexPun())),
           (((numSlotsUsqInt =
                  byteAt((void *)(array1 + (numSlotsFieldByteOffset()))))) ==
                    (numSlotsMask())
                ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                           (void *)(array1 - BaseHeaderSize))))
                                       << 8)))))) >>
                      8
                : numSlotsUsqInt))) ==
         ((/* begin numSlotsOf: */
           assert((classIndexOf(array2)) > (isForwardedObjectClassIndexPun())),
           (((numSlots =
                  byteAt((void *)(array2 + (numSlotsFieldByteOffset()))))) ==
                    (numSlotsMask())
                ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                           (void *)(array2 - BaseHeaderSize))))
                                       << 8)))))) >>
                      8
                : numSlots)))))) {
    return PrimErrBadArgument;
  }

  /* begin containsOnlyValidBecomeObjects:and:twoWay:copyHash: */
  /* begin lastPointerOf: */
  fmt = (byteAt((void *)(array1 + (formatFieldByteOffset())))) & (formatMask());
  assert(fmt != (forwardedFormat()));
  if (fmt <= 5 /* lastPointerFormat */) {
    if ((fmt == (indexablePointersFormat())) &&
        (((longAt((void *)(array1))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex)) {
      /* begin fetchStackPointerOf: */
      sp = fetchPointerofObject(StackPointerIndex, array1);
      if (!((((sp) & 7) == 1))) {
        contextSize = 0;
        goto l5;
      }
      assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(array1)));
      contextSize = (sp >> 3);
      /* end fetchStackPointerOf: */
    l5:
      fieldOffset = (((CtxtTempFrameStart - 1) + contextSize) * BytesPerOop) +
                    BaseHeaderSize;
      goto l2;
    }

    /* contexts end at the stack pointer */
    fieldOffset =
        ((((/* begin numSlotsOf: */
            assert((classIndexOf(array1)) > (isForwardedObjectClassIndexPun())),
            (((numSlots =
                   byteAt((void *)(array1 + (numSlotsFieldByteOffset()))))) ==
                     (numSlotsMask())
                 ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                            (void *)(array1 - BaseHeaderSize))))
                                        << 8)))))) >>
                       8
                 : numSlots))) -
          1) *
         BytesPerOop) +
        BaseHeaderSize;
    goto l2;
  }
  if (fmt < (firstCompiledMethodFormat())) {
    fieldOffset = 0;
    goto l2;
  }

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(array1));
  header = fetchPointerofObject(HeaderIndex, array1);
  fieldOffset = (((((/* begin literalCountOfMethodHeader: */
                     assert((((header) & 7) == 1)),
                     /* literalCountOfAlternateHeader: */
                     ((header >> 3)) & AlternateHeaderNumLiteralsMask)) +
                   LiteralStart) -
                  1) *
                 BytesPerOop) +
                BaseHeaderSize;
  /* end lastPointerOf: */
l2:
  size = 0;

  /* array1 is known to be the same size as array2 */
  while (fieldOffset >= BaseHeaderSize) {
    oop1 = longAt((void *)(array1 + fieldOffset));
    if (isOopForwarded(oop1)) {
      oop1 = followForwarded(oop1);
      longAtput((void *)(array1 + fieldOffset), oop1);
    }

    /* begin ifOopInvalidForBecome:errorCodeInto: */
    if (((oop1 & (tagMask())) != 0)) {
      ec = PrimErrInappropriate;
      goto l6;
    }
    if ((byteAt((void *)(oop1 + (formatFieldByteOffset())))) &
        (1U << (pinnedBitByteShift()))) {
      ec = PrimErrObjectIsPinned;
      goto l6;
    }
    if (
#if IMMUTABILITY
        ((((usqInt)((
              byteAt((void *)(oop1 + (immutableExtraBitsByteOffset())))))) >>
          (immutableBitByteShift())) &
         1) != 0
#else
        0
#endif
    ) {
      ec = PrimErrNoModification;
      goto l6;
    }
    /* end ifOopInvalidForBecome:errorCodeInto: */
  l3:
    oop2 = longAt((void *)(array2 + fieldOffset));
    if (isOopForwarded(oop2)) {
      oop2 = followForwarded(oop2);
      longAtput((void *)(array2 + fieldOffset), oop2);
    }
    if (oop1 != oop2) {
      if (twoWayFlag) {
        /* begin ifOopInvalidForBecome:errorCodeInto: */
        if (((oop2 & (tagMask())) != 0)) {
          ec = PrimErrInappropriate;
          goto l6;
        }
        if ((byteAt((void *)(oop2 + (formatFieldByteOffset())))) &
            (1U << (pinnedBitByteShift()))) {
          ec = PrimErrObjectIsPinned;
          goto l6;
        }
        if (
#if IMMUTABILITY
            ((((usqInt)((byteAt(
                  (void *)(oop2 + (immutableExtraBitsByteOffset())))))) >>
              (immutableBitByteShift())) &
             1) != 0
#else // IMMUTABILITY
            0
#endif
        ) {
          ec = PrimErrNoModification;
          goto l6;
        }
        /* end ifOopInvalidForBecome:errorCodeInto: */
      l4:
        size = (size + (bytesInBody(oop1))) + (bytesInBody(oop2));
        becomeEffectsFlags =
            (becomeEffectsFlags | (becomeEffectFlagsFor(oop1))) |
            (becomeEffectFlagsFor(oop2));
      } else {
        if (copyHashFlag) {
          if (((oop2 & (tagMask())) != 0)) {
            ec = PrimErrInappropriate;
            goto l6;
          }
          if (
#if IMMUTABILITY
              ((((usqInt)((byteAt(
                    (void *)(oop2 + (immutableExtraBitsByteOffset())))))) >>
                (immutableBitByteShift())) &
               1) != 0
#else // IMMUTABILITY
              0
#endif
          ) {
            ec = PrimErrNoModification;
            goto l6;
          }
        }
        becomeEffectsFlags = becomeEffectsFlags | (becomeEffectFlagsFor(oop1));
      }
    }
    fieldOffset -= BytesPerOop;
  }
  if (size >= (totalFreeOldSpace + (scavengeThreshold - freeStart))) {
    ec = PrimErrNoMemory;
    goto l6;
  }
  ec = 0;
  /* end containsOnlyValidBecomeObjects:and:twoWay:copyHash: */
l6:
  if (ec) {
    /* begin zeroBecomeEffectFlagsOnFailure */
    becomeEffectsFlags = 0;
    return ec;
  }

  /* begin preBecomeAction: */
  if (stackPage) {
    /* begin externalWriteBackHeadFramePointers */
    assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
    assert(stackPage == (mostRecentlyUsedPage));
    assert(!((isFree(stackPage))));

    /* begin setHeadFP:andSP:inPage: */
    assert(stackPointer < framePointer);
    assert((stackPointer < ((stackPage->baseAddress))) &&
           (stackPointer > (((stackPage->realStackLimit)) -
                            (LargeContextSlots * BytesPerOop))));
    assert((framePointer < ((stackPage->baseAddress))) &&
           (framePointer > (((stackPage->realStackLimit)) -
                            ((LargeContextSlots * BytesPerOop) / 2))));
    (stackPage->headFP = framePointer);
    (stackPage->headSP = stackPointer);
    assert(pageListIsWellFormed());
  }
  if (twoWayFlag) {
    /* begin innerBecomeObjectsIn:and:copyHash: */
    toDoLimit =
        ((assert((classIndexOf(array1)) > (isForwardedObjectClassIndexPun())),
          (((numSlots =
                 byteAt((void *)(array1 + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                          (void *)(array1 - BaseHeaderSize))))
                                      << 8)))))) >>
                     8
               : numSlots))) -
        1;
    for (i = 0; i <= toDoLimit; i += 1) {
      /* begin followField:ofObject: */
      obj1 = fetchPointerofObject(i, array1);
      if (isOopForwarded(obj1)) {
        obj1 = fixFollowedFieldofObjectwithInitialValue(i, array1, obj1);
      }

      /* begin followField:ofObject: */
      obj2 = fetchPointerofObject(i, array2);
      if (isOopForwarded(obj2)) {
        obj2 = fixFollowedFieldofObjectwithInitialValue(i, array2, obj2);
      }
      if (obj1 != obj2) {
        /* begin doBecome:and:copyHash: */
        o1ClassIndex =
            (long32At((void *)(obj1 + 4))) & (identityHashHalfWordMask());
        if ((o1ClassIndex != 0) && ((classAtIndex(o1ClassIndex)) != obj1)) {
          o1ClassIndex = 0;
        }
        o2ClassIndex =
            (long32At((void *)(obj2 + 4))) & (identityHashHalfWordMask());
        if ((o2ClassIndex != 0) && ((classAtIndex(o2ClassIndex)) != obj2)) {
          o2ClassIndex = 0;
        }

        /* Refuse to do an in-place become on classes since their being
           forwarded is used in the flush method cache implementations. */
        if ((((/* begin numSlotsOf: */
               assert((classIndexOf(obj1)) >
                      (isForwardedObjectClassIndexPun())),
               (((numSlotsUsqInt =
                      byteAt((void *)(obj1 + (numSlotsFieldByteOffset()))))) ==
                        (numSlotsMask())
                    ? ((((usqInt)(((sqInt)((usqInt)((longAt((
                                               void *)(obj1 - BaseHeaderSize))))
                                           << 8)))))) >>
                          8
                    : numSlotsUsqInt))) ==
             ((/* begin numSlotsOf: */
               assert((classIndexOf(obj2)) >
                      (isForwardedObjectClassIndexPun())),
               (((numSlots =
                      byteAt((void *)(obj2 + (numSlotsFieldByteOffset()))))) ==
                        (numSlotsMask())
                    ? ((((usqInt)(((sqInt)((usqInt)((longAt((
                                               void *)(obj2 - BaseHeaderSize))))
                                           << 8)))))) >>
                          8
                    : numSlots)))) &&
            ((o1ClassIndex + o2ClassIndex) == 0)) {
          inPlaceBecomeandcopyHashFlag(obj1, obj2, copyHashFlag);
          goto l1;
        }
        outOfPlaceBecomeandcopyHashFlag(obj1, obj2, copyHashFlag);

        /* if copyHashFlag then nothing changes, since hashes were also swapped.
         */
        if (copyHashFlag) {
          goto l1;
        }

        /* if copyHash is false then the classTable entries must be updated.
           We leave the following until postBecomeScanClassTable:, but must
           swap the forwarders if two active classes have been becommed,
           and assign hashes if not. */
        if (o1ClassIndex) {
          if (o2ClassIndex) {
            classAtIndexput(o1ClassIndex, obj2);
            classAtIndexput(o2ClassIndex, obj1);
          } else {
            newObj2 = followForwarded(obj1);
            assert((rawHashBitsOf(newObj2)) == 0);

            /* begin setHashBitsOf:to: */
            long32Atput((void *)(newObj2 + 4),
                        ((((long32At((void *)(newObj2 + 4))) |
                           (identityHashHalfWordMask())) -
                          (identityHashHalfWordMask()))) +
                            (o1ClassIndex & (identityHashHalfWordMask())));
          }
        } else {
          if (o2ClassIndex) {
            newObj1 = followForwarded(obj2);
            assert((rawHashBitsOf(newObj1)) == 0);

            /* begin setHashBitsOf:to: */
            long32Atput((void *)(newObj1 + 4),
                        ((((long32At((void *)(newObj1 + 4))) |
                           (identityHashHalfWordMask())) -
                          (identityHashHalfWordMask()))) +
                            (o2ClassIndex & (identityHashHalfWordMask())));
          }
        }
        /* end doBecome:and:copyHash: */
      l1:
        objOop = fetchPointerofObject(i, array1);
        if (isOopForwarded(objOop)) {
          objOop = fixFollowedFieldofObjectwithInitialValue(i, array1, objOop);
        }

        /* begin followField:ofObject: */
        objOopSqInt = fetchPointerofObject(i, array2);
        if (isOopForwarded(objOopSqInt)) {
          objOopSqInt =
              fixFollowedFieldofObjectwithInitialValue(i, array2, objOopSqInt);
        }
      }
    }
  } else {
    /* begin innerBecomeObjectsIn:to:copyHash: */
    toDoLimit =
        ((assert((classIndexOf(array1)) > (isForwardedObjectClassIndexPun())),
          (((numSlots =
                 byteAt((void *)(array1 + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                          (void *)(array1 - BaseHeaderSize))))
                                      << 8)))))) >>
                     8
               : numSlots))) -
        1;
    for (i = 0; i <= toDoLimit; i += 1) {
      /* begin followField:ofObject: */
      obj1 = fetchPointerofObject(i, array1);
      if (isOopForwarded(obj1)) {
        obj1 = fixFollowedFieldofObjectwithInitialValue(i, array1, obj1);
      }

      /* begin followField:ofObject: */
      obj2 = fetchPointerofObject(i, array2);
      if (isOopForwarded(obj2)) {
        obj2 = fixFollowedFieldofObjectwithInitialValue(i, array2, obj2);
      }
      if (obj1 != obj2) {
        /* begin doBecome:to:copyHash: */
        /* begin forward:to: */
        /* begin set:classIndexTo:formatTo: */
        assert((((isForwardedObjectClassIndexPun()) >= 0) &&
                ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
        assert((((forwardedFormat()) >= 0) &&
                ((forwardedFormat()) <= (formatMask()))));
        longAtput(
            (void *)(obj1),
            ((longAt((void *)(obj1))) &
             (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) +
                             (classIndexMask())))) +
                ((isForwardedObjectClassIndexPun()) +
                 ((((usqInt)((forwardedFormat())) << (formatShift()))))));
#if IMMUTABILITY
        /* begin setIsImmutableOf:to: */
        longAtput((void *)(obj1),
                  (longAt((void *)(obj1))) &
                      (~(usqIntptr_t)(1U << (immutableBitShift()))));
#endif // IMMUTABILITY

        assert(isForwarded(obj1));
        assert(!(isOopForwarded(obj2)));

        /* begin isOldObject: */
        assert(isNonImmediate(obj1));
        if (oopisGreaterThanOrEqualTo(obj1, oldSpaceStart)) {
          if (/* isYoung: */
              ((!(obj2 & (tagMask())))) &&
              (oopisLessThan(obj2, oldSpaceStart))) {
            /* begin possibleRootStoreInto: */
            if (!((byteAt((void *)(obj1 + (formatFieldByteOffset())))) &
                  (1U << (rememberedBitByteShift())))) {
              remember(obj1);
            }
          }
        }

        /* most stores into young objects */
        longAtput((void *)((obj1 + BaseHeaderSize) + (0U << (shiftForWord()))),
                  obj2);

        /* For safety make sure the forwarder has a slot count that includes its
         * contents. */
        if (!(byteAt((void *)(obj1 + (numSlotsFieldByteOffset()))))) {
          /* rawNumSlotsOf:put: */
          byteAtput((void *)(obj1 + (numSlotsFieldByteOffset())), 1);
        }
        if (copyHashFlag) {
          hash = (long32At((void *)(obj1 + 4))) & (identityHashHalfWordMask());

          /* begin setHashBitsOf:to: */
          long32Atput((void *)(obj2 + 4),
                      ((((long32At((void *)(obj2 + 4))) |
                         (identityHashHalfWordMask())) -
                        (identityHashHalfWordMask()))) +
                          (hash & (identityHashHalfWordMask())));
        }
        if (((/* begin isOldObject: */
              assert(isNonImmediate(obj1)),
              oopisGreaterThanOrEqualTo(obj1, oldSpaceStart))) &&
            (/* isYoung: */
             ((!(obj2 & (tagMask())))) &&
             (oopisLessThan(obj2, oldSpaceStart)))) {
          becomeEffectsFlags = becomeEffectsFlags | OldBecameNewFlag;
        }
        assert(!((isOopForwarded(obj2))));

        /* begin followField:ofObject: */
        objOop = fetchPointerofObject(i, array1);
        if (isOopForwarded(objOop)) {
          objOop = fixFollowedFieldofObjectwithInitialValue(i, array1, objOop);
        }
        assert(!(isOopForwarded(obj2)));
      }
    }
  }

  /* begin followSpecialObjectsOop */
  if ((!((longAt((void *)(specialObjectsOop))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    validatedIntegerClassFlags = 0;

    specialObjectsOop = followForwarded(specialObjectsOop);
  }
  followForwardedObjectFieldstoDepth(specialObjectsOop, 0);

  /* N.B. perform coInterpreter's postBecomeAction: *before*
     postBecomeScanClassTable: to allow the coInterpreter to void method cache
     entries by spotting classIndices that refer to forwarded objects.
     postBecomeScanClassTable: follows forwarders in the table. */

  /* begin postBecomeAction: */
  spurPostBecomeAction(becomeEffectsFlags);
  postBecomeScanClassTable(becomeEffectsFlags);
  if (twoWayFlag) {
    assert(validPostBecomeArrayContents(array1));
    assert(validPostBecomeArrayContents(array2));
  } else {
    assert(validPostBecomeArrayContents(array2));
  }
  assert(validClassTableHashes());
  runLeakCheckerFor(GCModeBecome);

  /* begin zeroBecomeEffectFlagsOnSuccess */
  becomeEffectsFlags = 0;
  return PrimNoErr;
}
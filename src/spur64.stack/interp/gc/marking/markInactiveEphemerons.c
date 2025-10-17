/* Extracted from interp.c:39866 (function markInactiveEphemerons). */

/*	Go through the unscanned ephemerons, marking the inactive ones, and
        removing them from the unscanned ephemerons. Answer if any inactive
        ones were found. We cannot fire the ephemerons until all are found to
        be active since scan-marking an inactive ephemeron later in the set may
        render a previously-observed active ephemeron as inactive. */

/* SpurMemoryManager>>#markInactiveEphemerons */

static sqInt markInactiveEphemerons(void) {
  sqInt contextSize;
  sqInt ephemeron;
  sqInt field;
  sqInt fmt;
  sqInt format;
  sqInt formatSqInt;
  sqInt foundInactive;
  sqInt header;
  sqInt index;
  sqInt key;
  usqInt numLiterals;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt numStrongSlots;
  sqInt objOop;
  sqInt objOop1;
  sqInt objOopSqInt;
  sqInt objOopSqIntSqInt;
  sqInt objStack;
  sqInt objStack1;
  sqInt objStackSqInt;
  sqInt objToScan;
  usqInt ptr;
  sqInt scanLargeObject;
  sqInt sp;

  foundInactive = 0;
  ptr = (unscannedEphemerons.start);
  while (ptr <= ((unscannedEphemerons.top))) {
    objOop = (ephemeron = longAt((void *)(ptr)));

    /* begin followedKeyOfEphemeron: */
    assert((isNonImmediate(objOop)) && (isEphemeron(objOop)));

    /* begin followOopField:ofObject: */
    key =
        longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
    if (isOopForwarded(key)) {
      key = fixFollowedFieldofObjectwithInitialValue(0, objOop, key);
    }
    if ((((key & (tagMask())) != 0)) ||
        (((byteAt((void *)(key + (markBitsByteOffset())))) &
          (1U << (markedBitByteShift()))) != 0)) {
      foundInactive = 1;

      /* Now remove the inactive ephemeron from the set, and trace it.
         Tracing it may add more ephemerons to the set. */
      if (((unscannedEphemerons.top)) > ptr) {
        longAtput((void *)(ptr), longAt((void *)((unscannedEphemerons.top))));
      }
      (unscannedEphemerons.top = ((unscannedEphemerons.top)) - BytesPerOop);

      /* To get markAndTrace: to trace the ephemeron we need to clear the mark
       * bit. */

      /* begin markLoopFrom: */
      numStrongSlots = 0;
      objToScan = ephemeron;

      /* To avoid overflowing the mark stack when we encounter large objects, we
         push the obj, then its numStrongSlots, and then index the object from
         the stack. */
      do {
        if (((objToScan & (tagMask())) != 0)) {
          scanLargeObject = 1;
        } else {
          /* begin numStrongSlotsOfInephemeral: */
          fmt = (byteAt((void *)(objToScan + (formatFieldByteOffset())))) &
                (formatMask());
          assert((fmt != (ephemeronFormat())) ||
                 ((isImmediate(keyOfEphemeron(objToScan))) ||
                  (isMarked(keyOfEphemeron(objToScan)))));
          if (fmt <= 5 /* lastPointerFormat */) {
            /* begin numSlotsOf: */
            assert((classIndexOf(objToScan)) >
                   (isForwardedObjectClassIndexPun()));
            numSlots =
                (((numSlotsUsqInt = byteAt(
                       (void *)(objToScan + (numSlotsFieldByteOffset()))))) ==
                         (numSlotsMask())
                     ? ((((usqInt)((
                           (sqInt)((usqInt)((longAt(
                                       (void *)(objToScan - BaseHeaderSize))))
                                   << 8)))))) >>
                           8
                     : numSlotsUsqInt);
            if (fmt <= (arrayFormat())) {
              numStrongSlots = numSlots;
              goto l4;
            }
            if (fmt == (indexablePointersFormat())) {
              if (((longAt((void *)(objToScan))) & (classIndexMask())) ==
                  ClassMethodContextCompactIndex) {
                setTraceFlagOnContextsFramesPageIfNeeded(objToScan);

                /* contexts end at the stack pointer */

                /* begin fetchStackPointerOf: */
                sp = longAt((void *)((objToScan + BaseHeaderSize) +
                                     ((((usqInt)(StackPointerIndex)
                                        << (shiftForWord()))))));
                if (!((((sp) & 7) == 1))) {
                  contextSize = 0;
                  goto l3;
                }
                assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(objToScan)));
                contextSize = (sp >> 3);
                /* end fetchStackPointerOf: */
              l3:
                numStrongSlots = CtxtTempFrameStart + contextSize;
                goto l4;
              }
              numStrongSlots = numSlots;
              goto l4;
            }
            if (fmt == (weakArrayFormat())) {
              objOopSqIntSqInt = fetchClassOfNonImm(objToScan);
              numStrongSlots =
                  (((longAt((void *)((objOopSqIntSqInt + BaseHeaderSize) +
                                     ((((usqInt)(InstanceSpecificationIndex)
                                        << (shiftForWord()))))))) >>
                    3)) &
                  ((1U << (fixedFieldsFieldWidth())) - 1);
              goto l4;
            }
            numStrongSlots = numSlots;
            goto l4;
          }
          if (fmt == (forwardedFormat())) {
            numStrongSlots = 1;
            goto l4;
          }
          if (fmt < (firstCompiledMethodFormat())) {
            numStrongSlots = 0;
            goto l4;
          }

          /* begin methodHeaderOf: */
          assert(isCompiledMethod(objToScan));
          header =
              longAt((void *)((objToScan + BaseHeaderSize) +
                              ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

          /* begin literalCountOfMethodHeader: */
          assert((((header) & 7) == 1));
          numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
          numStrongSlots = numLiterals + LiteralStart;
          /* end numStrongSlotsOfInephemeral: */
        l4:
          scanLargeObject = numStrongSlots > 64 /* traceImmediatelySlotLimit */;
        }
        if (scanLargeObject) {
          if (((objToScan & (tagMask())) != 0)) {
            index = (objToScan >> 3);
            objToScan = topOfObjStack(markStack);
          } else {
            index = numStrongSlots;
            markAndTraceClassOf(objToScan);
          }
          while (index > 0) {
            index -= 1;
            field = longAt((void *)((objToScan + BaseHeaderSize) +
                                    ((((usqInt)(index) << (shiftForWord()))))));
            if ((!(field & (tagMask())))) {
              if ((!((longAt((void *)(field))) &
                     ((classIndexMask()) -
                      (isForwardedObjectClassIndexPun()))))) {
                field = fixFollowedFieldofObjectwithInitialValue(
                    index, objToScan, field);
              }

              /* fixFollowedField: is /not/ inlined */

              /* begin markAndShouldScan: */
              if (((field & (tagMask())) != 0)) {
                goto l1;
              }

              /* if markAndTrace: is to follow and eliminate forwarding pointers
                 in its scan it cannot be handed an r-value which is forwarded.
               */
              assert(!(isForwarded(field)));
              if ((byteAt((void *)(field + (markBitsByteOffset())))) &
                  (1U << (markedBitByteShift()))) {
                goto l1;
              }

              /* begin setIsMarkedOf:to: */
              assert(!(isFreeObject(field)));
              byteAtput((void *)(field + (markBitsByteOffset())),
                        (byteAt((void *)(field + (markBitsByteOffset())))) |
                            (1U << (markedBitByteShift())));
              format = (byteAt((void *)(field + (formatFieldByteOffset())))) &
                       (formatMask());
              if (/* isPureBitsFormat: */
                  (format >= (sixtyFourBitIndexableFormat())) &&
                  (format < (firstCompiledMethodFormat()))) {
                if (((longAt((void *)(field))) & (classIndexMask())) >
                    (lastClassIndexPun())) {
                  markAndTraceClassOf(field);
                }
                goto l1;
              }

              /* avoid pushing non-pointer objects on the markStack.
                 Avoid tracing classes of non-objects on the heap, e.g. IRC
                 caches, Sista counters. */
              if (format == (weakArrayFormat())) {
                objStack1 = weaklingStack;

                /* begin push:onObjStack: */
                assert(addressCouldBeOop(field));
                if (((field & (tagMask())) != 0)) {
                  assert(objStack1 == markStack);
                  assert(addressCouldBeObj(topOfObjStack(
                      (0 == (fetchPointerofObject(ObjStackTopx, objStack1))
                           ? fetchPointerofObject(ObjStackNextx, objStack1)
                           : objStack1))));
                } else {
                  assert(!((objStack1 == markStack) && (isWeakNonImm(field))));

                  /* There should only be weaklings on the weaklingStack */
                  assert((objStack1 != weaklingStack) || (isWeakNonImm(field)));
                }

                /* There should be no weaklings on the mark stack. */
                noCheckPushonObjStack(field, objStack1);
                goto l1;
              }

              /* push weaklings on the weakling stack to scan later */
              if ((format == (ephemeronFormat())) &&
                  (activeAndDeferredScan(field))) {
                goto l1;
              }
              if (index > 0) {
                if ((topOfObjStack(markStack)) != objToScan) {
                  objStack = markStack;

                  /* begin push:onObjStack: */
                  assert(addressCouldBeOop(objToScan));
                  if (((objToScan & (tagMask())) != 0)) {
                    assert(objStack == markStack);
                    assert(addressCouldBeObj(topOfObjStack(
                        (0 == (fetchPointerofObject(ObjStackTopx, objStack))
                             ? fetchPointerofObject(ObjStackNextx, objStack)
                             : objStack))));
                  } else {
                    assert(!((objStack == markStack) &&
                             (isWeakNonImm(objToScan))));

                    /* There should only be weaklings on the weaklingStack */
                    assert((objStack != weaklingStack) ||
                           (isWeakNonImm(objToScan)));
                  }

                  /* There should be no weaklings on the mark stack. */
                  noCheckPushonObjStack(objToScan, objStack);
                }
                objOopSqInt = (((usqInt)index << 3) | 1);
                objStackSqInt = markStack;

                /* begin push:onObjStack: */
                assert(addressCouldBeOop(objOopSqInt));
                if (((objOopSqInt & (tagMask())) != 0)) {
                  assert(objStackSqInt == markStack);
                  assert(addressCouldBeObj(topOfObjStack(
                      (0 == (fetchPointerofObject(ObjStackTopx, objStackSqInt))
                           ? fetchPointerofObject(ObjStackNextx, objStackSqInt)
                           : objStackSqInt))));
                } else {
                  assert(!((objStackSqInt == markStack) &&
                           (isWeakNonImm(objOopSqInt))));

                  /* There should only be weaklings on the weaklingStack */
                  assert((objStackSqInt != weaklingStack) ||
                         (isWeakNonImm(objOopSqInt)));
                }

                /* There should be no weaklings on the mark stack. */
                noCheckPushonObjStack(objOopSqInt, objStackSqInt);
              }
              objToScan = field;
              index = -1;
            l1:;
            }
          }
          if (index >= 0) {
            objToScan = popObjStack(markStack);
            if (objToScan == ephemeron) {
              objToScan = popObjStack(markStack);
            }
          }
        } else {
          index = numStrongSlots;
          markAndTraceClassOf(objToScan);
          while (index > 0) {
            index -= 1;
            field = longAt((void *)((objToScan + BaseHeaderSize) +
                                    ((((usqInt)(index) << (shiftForWord()))))));
            if ((!(field & (tagMask())))) {
              if ((!((longAt((void *)(field))) &
                     ((classIndexMask()) -
                      (isForwardedObjectClassIndexPun()))))) {
                field = fixFollowedFieldofObjectwithInitialValue(
                    index, objToScan, field);
              }

              /* fixFollowedField: is /not/ inlined */

              /* begin markAndShouldScan: */
              if (((field & (tagMask())) != 0)) {
                goto l2;
              }

              /* if markAndTrace: is to follow and eliminate forwarding pointers
                 in its scan it cannot be handed an r-value which is forwarded.
               */
              assert(!(isForwarded(field)));
              if ((byteAt((void *)(field + (markBitsByteOffset())))) &
                  (1U << (markedBitByteShift()))) {
                goto l2;
              }

              /* begin setIsMarkedOf:to: */
              assert(!(isFreeObject(field)));
              byteAtput((void *)(field + (markBitsByteOffset())),
                        (byteAt((void *)(field + (markBitsByteOffset())))) |
                            (1U << (markedBitByteShift())));
              formatSqInt =
                  (byteAt((void *)(field + (formatFieldByteOffset())))) &
                  (formatMask());
              if (/* isPureBitsFormat: */
                  (formatSqInt >= (sixtyFourBitIndexableFormat())) &&
                  (formatSqInt < (firstCompiledMethodFormat()))) {
                if (((longAt((void *)(field))) & (classIndexMask())) >
                    (lastClassIndexPun())) {
                  markAndTraceClassOf(field);
                }
                goto l2;
              }

              /* avoid pushing non-pointer objects on the markStack.
                 Avoid tracing classes of non-objects on the heap, e.g. IRC
                 caches, Sista counters. */
              if (formatSqInt == (weakArrayFormat())) {
                objStack1 = weaklingStack;

                /* begin push:onObjStack: */
                assert(addressCouldBeOop(field));
                if (((field & (tagMask())) != 0)) {
                  assert(objStack1 == markStack);
                  assert(addressCouldBeObj(topOfObjStack(
                      (0 == (fetchPointerofObject(ObjStackTopx, objStack1))
                           ? fetchPointerofObject(ObjStackNextx, objStack1)
                           : objStack1))));
                } else {
                  assert(!((objStack1 == markStack) && (isWeakNonImm(field))));

                  /* There should only be weaklings on the weaklingStack */
                  assert((objStack1 != weaklingStack) || (isWeakNonImm(field)));
                }

                /* There should be no weaklings on the mark stack. */
                noCheckPushonObjStack(field, objStack1);
                goto l2;
              }

              /* push weaklings on the weakling stack to scan later */
              if ((formatSqInt == (ephemeronFormat())) &&
                  (activeAndDeferredScan(field))) {
                goto l2;
              }
              objStack1 = markStack;

              /* begin push:onObjStack: */
              assert(addressCouldBeOop(field));
              if (((field & (tagMask())) != 0)) {
                assert(objStack1 == markStack);
                assert(addressCouldBeObj(topOfObjStack(
                    (0 == (fetchPointerofObject(ObjStackTopx, objStack1))
                         ? fetchPointerofObject(ObjStackNextx, objStack1)
                         : objStack1))));
              } else {
                assert(!((objStack1 == markStack) && (isWeakNonImm(field))));

                /* There should only be weaklings on the weaklingStack */
                assert((objStack1 != weaklingStack) || (isWeakNonImm(field)));
              }

              /* There should be no weaklings on the mark stack. */
              noCheckPushonObjStack(field, objStack1);
              if (((byteAt((void *)(field + (numSlotsFieldByteOffset())))) >
                   64 /* traceImmediatelySlotLimit */) &&
                  (((numStrongSlots = numStrongSlotsOfInephemeral(field))) >
                   64 /* traceImmediatelySlotLimit */)) {
                objOop1 = (((usqInt)numStrongSlots << 3) | 1);
                objStack1 = markStack;

                /* begin push:onObjStack: */
                assert(addressCouldBeOop(objOop1));
                if (((objOop1 & (tagMask())) != 0)) {
                  assert(objStack1 == markStack);
                  assert(addressCouldBeObj(topOfObjStack(
                      (0 == (fetchPointerofObject(ObjStackTopx, objStack1))
                           ? fetchPointerofObject(ObjStackNextx, objStack1)
                           : objStack1))));
                } else {
                  assert(
                      !((objStack1 == markStack) && (isWeakNonImm(objOop1))));

                  /* There should only be weaklings on the weaklingStack */
                  assert((objStack1 != weaklingStack) ||
                         (isWeakNonImm(objOop1)));
                }

                /* There should be no weaklings on the mark stack. */
                noCheckPushonObjStack(objOop1, objStack1);
              }
            l2:;
            }
          }
          objToScan = popObjStack(markStack);
        }

        /* scanning a large object. scan until hitting an unmarked object, then
           switch to it, if any. scanning a small object. scan, marking, pushing
           unmarked referents, then switch to the top of the stack. */
      } while (objToScan);
    } else {
      ptr += BytesPerOop;
    }
  }
  return foundInactive;
}
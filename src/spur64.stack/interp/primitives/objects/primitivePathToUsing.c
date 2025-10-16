/* Extracted from interp.c:20095 (function primitivePathToUsing). */

/*	primitivePathTo: anObject using: stack <Array> followWeak: boolean
        Answer a path to anObject from the root that does not pass through
        the current context */

/* InterpreterPrimitives>>#primitivePathToUsing */

EXPORT(sqInt)
primitivePathToUsing(void) {
  sqInt contextSize;
  sqInt current;
  sqInt err;
  sqInt fmt;
  usqInt freeStartAtStart;
  sqInt header;
  sqInt i;
  sqInt index;
  sqInt iSqInt;
  sqInt next;
  usqInt numLiterals;
  sqInt path;
  sqInt spSqInt;
  sqInt stack;
  sqInt stackp;
  sqInt stackSize;
  sqInt toDoLimit;
  sqInt toDoLimitSqInt;

  err = 0;

  externalWriteBackHeadFramePointers();
  if (!(argumentCount >= 2)) {
    return (primFailCode = PrimErrBadNumArgs);
  }
  if (!(((longAt(stackPointer)) == trueObj) ||
        ((longAt(stackPointer)) == falseObj))) {
    return (primFailCode = PrimErrBadArgument);
  }
  stack = longAt(stackPointer + (1 * BytesPerWord));

  /* begin pathTo:using:followWeak: */
  if (!(/* isArray: */
        ((!(stack & (tagMask())))) &&
        (((byteAt((void *)(stack + (formatFieldByteOffset())))) &
          (formatMask())) == (arrayFormat())))) {
    err = PrimErrBadArgument;
    goto l5;
  }
  assert(allObjectsUnmarked());

  /* check no allocations during search */
  freeStartAtStart = freeStart;
  beRootIfOld(stack);

  stackSize = lengthOf(stack);
l1:

  /* begin mark: */
  /* begin setIsMarkedOf:to: */
  assert(!(isFreeObject(stack)));
  byteAtput((void *)(stack + (markBitsByteOffset())),
            (byteAt((void *)(stack + (markBitsByteOffset())))) |
                (1U << (markedBitByteShift())));

  /* no need. the current context is not reachable from the active process
     (suspendedContext is nil) objectMemory mark: self activeProcess. */
  current = specialObjectsOop;

  /* begin mark: */
  /* begin setIsMarkedOf:to: */
  assert(!(isFreeObject(current)));
  byteAtput((void *)(current + (markBitsByteOffset())),
            (byteAt((void *)(current + (markBitsByteOffset())))) |
                (1U << (markedBitByteShift())));

  index = lengthOf(current);
l2:
  stackp = 0;
  while (1) {
    while (((index -= 1)) >= -1) {
      if (/* couldBeFramePointer: */
          (((((usqInt)current)) & (BytesPerWord - 1)) == 0) &&
          ((((((usqInt)current)) >= (((usqInt)stackMemory))) &&
            ((((usqInt)current)) <= (((usqInt)pages)))))) {
        next = (index >= 0 ? fieldofFrame(index, ((char *)current)) : nilObj);
      } else {
        if (index >= 0) {
          next = (((longAt((void *)(current))) & (classIndexMask())) ==
                          ClassMethodContextCompactIndex
                      ? fieldOrSenderFPofContext(index, current)
                      : fetchPointerofObject(index, current));
        } else {
          next = fetchClassOfNonImm(current);
        }
      }
      if (/* couldBeFramePointer: */
          (((((usqInt)next)) & (BytesPerWord - 1)) == 0) &&
          ((((((usqInt)next)) >= (((usqInt)stackMemory))) &&
            ((((usqInt)next)) <= (((usqInt)pages)))))) {
        assert(isFrameonPage(((char *)next), stackPageFor(((char *)next))));
      } else {
        assert(checkOkayOop(next));
      }
      if (next == (longAt(stackPointer + (2 * BytesPerWord)))) {
        assert(freeStartAtStart == (freeStart));
        unmarkAfterPathTo();

        storePointerofObjectwithValue(stackp, stack, current);
        pruneStackstackp(stack, stackp);
        err = 0;
        goto l5;
      }
      if (((!(next & (smallIntegerTag())))) &&
          ((/* couldBeFramePointer: */
            (((((usqInt)next)) & (BytesPerWord - 1)) == 0) &&
                    ((((((usqInt)next)) >= (((usqInt)stackMemory))) &&
                      ((((usqInt)next)) <= (((usqInt)pages)))))
                ? !(((longAt((void *)(next + FoxFrameFlags))) & 2) != 0)
                : (!(((byteAt((void *)(next + (markBitsByteOffset())))) &
                      (1U << (markedBitByteShift()))) != 0)) &&
                      (((/* isPointers: */
                         ((!(next & (tagMask())))) &&
                         (((byteAt(
                               (void *)(next + (formatFieldByteOffset())))) &
                           (formatMask())) <= 5 /* lastPointerFormat */)) ||
                        (((byteAt((void *)(next + (formatFieldByteOffset())))) &
                          (formatMask())) >= (firstCompiledMethodFormat()))) &&
                       (((longAt(stackPointer)) == trueObj) ||
                        (!(isWeakNonImm(next)))))))) {
        if ((stackp + 2) > stackSize) {
          assert(freeStartAtStart == (freeStart));
          unmarkAfterPathTo();

          /* begin nilFieldsOf: */
          toDoLimit = ((assert((classIndexOf(stack)) >
                               (isForwardedObjectClassIndexPun())),
                        numSlotsOf(stack))) -
                      1;
          for (i = 0; i <= toDoLimit; i += 1) {
            /* begin storePointerUnchecked:ofObject:withValue: */
            assert((isNonImmediate(stack)) && (!(isForwarded(stack))));
            assert(validStorePointerUncheckedArgs(i, stack, nilObj));
            longAtput((void *)((stack + BaseHeaderSize) +
                               ((((usqInt)(i) << (shiftForWord()))))),
                      nilObj);
          }
          err = PrimErrBadIndex;
          goto l5;
        }

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(stack)) && (!(isForwarded(stack))));
        assert(validStorePointerUncheckedArgs(stackp, stack, current));
        longAtput((void *)((stack + BaseHeaderSize) +
                           ((((usqInt)(stackp) << (shiftForWord()))))),
                  current);

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(stack)) && (!(isForwarded(stack))));
        assert(validStorePointerUncheckedArgs(stackp + 1, stack,
                                              (((usqInt)index << 3) | 1)));
        longAtput((void *)((stack + BaseHeaderSize) +
                           ((((usqInt)((stackp + 1)) << (shiftForWord()))))),
                  (((usqInt)index << 3) | 1));
        stackp += 2;
        if (/* couldBeFramePointer: */
            (((((usqInt)(((char *)next)))) & (BytesPerWord - 1)) == 0) &&
            ((((((usqInt)(((char *)next)))) >= (((usqInt)stackMemory))) &&
              ((((usqInt)(((char *)next)))) <= (((usqInt)pages)))))) {
          /* begin markFrame: */
          longAtput((void *)(next + FoxFrameFlags),
                    (longAt((void *)(next + FoxFrameFlags))) | 2);
          index =
              CtxtTempFrameStart + (stackPointerIndexForFrame(((char *)next)));
        } else {
          /* begin mark: */
          /* begin setIsMarkedOf:to: */
          assert(!(isFreeObject(next)));
          byteAtput((void *)(next + (markBitsByteOffset())),
                    (byteAt((void *)(next + (markBitsByteOffset())))) |
                        (1U << (markedBitByteShift())));

          /* begin numPointerSlotsOf: */
          fmt = (byteAt((void *)(next + (formatFieldByteOffset())))) &
                (formatMask());
          if (fmt <= 5 /* lastPointerFormat */) {
            if ((fmt == (indexablePointersFormat())) &&
                (((longAt((void *)(next))) & (classIndexMask())) ==
                 ClassMethodContextCompactIndex)) {
              /* begin fetchStackPointerOf: */
              spSqInt = fetchPointerofObject(StackPointerIndex, next);
              if (!((((spSqInt) & 7) == 1))) {
                contextSize = 0;
                goto l3;
              }
              assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(next)));
              contextSize = (spSqInt >> 3);
              /* end fetchStackPointerOf: */
            l3:
              index = ((usqInt)(CtxtTempFrameStart + contextSize));
              goto l4;
            }

            /* contexts end at the stack pointer */

            index = numSlotsOf(next);
            goto l4;
          }
          if (fmt == (forwardedFormat())) {
            index = 1;
            goto l4;
          }
          if (fmt < (firstCompiledMethodFormat())) {
            index = 0;
            goto l4;
          }

          header = methodHeaderOf(next);

          /* begin literalCountOfMethodHeader: */
          assert((((header) & 7) == 1));
          numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
          index = numLiterals + LiteralStart;
          /* end numPointerSlotsOf: */
        l4:;
        }
        current = next;
      }
    }
    if (current == specialObjectsOop) {
      assert(freeStartAtStart == (freeStart));
      unmarkAfterPathTo();

      /* begin nilFieldsOf: */
      toDoLimitSqInt =
          ((assert((classIndexOf(stack)) > (isForwardedObjectClassIndexPun())),
            numSlotsOf(stack))) -
          1;
      for (iSqInt = 0; iSqInt <= toDoLimitSqInt; iSqInt += 1) {
        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(stack)) && (!(isForwarded(stack))));
        assert(validStorePointerUncheckedArgs(iSqInt, stack, nilObj));
        longAtput((void *)((stack + BaseHeaderSize) +
                           ((((usqInt)(iSqInt) << (shiftForWord()))))),
                  nilObj);
      }
      err = PrimErrNotFound;
      goto l5;
    }
    index = ((fetchPointerofObject(stackp - 1, stack)) >> 3);
    current = fetchPointerofObject(stackp - 2, stack);
    stackp -= 2;
  }
  /* end pathTo:using:followWeak: */
l5:
  if (err) {
    return (primFailCode = err);
  }
  path = longAt(stackPointer + (1 * BytesPerWord));

  /* begin pop:thenPush: */
  popthenPush(argumentCount + 1, path);
  return 0;
}
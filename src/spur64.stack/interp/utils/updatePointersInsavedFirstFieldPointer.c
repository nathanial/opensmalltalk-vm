/* Extracted from interp.c:48754 (function
 * updatePointersInsavedFirstFieldPointer). */

/*	Sweep the pointer fields in obj, updating all references to mobile
   objects to their eventual locations. firstFieldPtr is supplied for mobile
   objects so that the saved first field can be updated, and so that the first
   field of a compiled method (which is its header, or reference to a CogMethod
   holding its header) can be retrieved. */

/* SpurPlanningCompactor>>#updatePointersIn:savedFirstFieldPointer: */

static void updatePointersInsavedFirstFieldPointer(sqInt obj,
                                                   sqInt firstFieldPtr) {
  sqInt contextSize;
  sqInt fmt;
  sqInt fwd;
  sqInt header;
  sqInt i;
  usqInt numLiterals;
  sqInt numPointerSlots;
  usqInt numSlots;
  sqInt oop;
  sqInt sp;

  fmt = (byteAt((void *)(obj + (formatFieldByteOffset())))) & (formatMask());

  /* begin numPointerSlotsWhileCompactingOf:withFormat:savedFirstFieldPointer:
   */
  assert((firstFieldPtr) && (isMobile(obj)));
  if (fmt <= 5 /* lastPointerFormat */) {
    if ((fmt == (indexablePointersFormat())) &&
        (((longAt((void *)(obj))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex)) {
      /* begin fetchStackPointerOf: */
      sp = longAt(
          (void *)((obj + BaseHeaderSize) +
                   ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
      if (!((((sp) & 7) == 1))) {
        contextSize = 0;
        goto l1;
      }
      assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(obj)));
      contextSize = (sp >> 3);
      /* end fetchStackPointerOf: */
    l1:
      numPointerSlots = ((usqInt)(CtxtTempFrameStart + contextSize));
      goto l2;
    }

    /* contexts end at the stack pointer */

    /* begin numSlotsOf: */
    assert((classIndexOf(obj)) > (isForwardedObjectClassIndexPun()));
    numPointerSlots =
        (((numSlots = byteAt((void *)(obj + (numSlotsFieldByteOffset()))))) ==
                 (numSlotsMask())
             ? ((((usqInt)((
                   (sqInt)((usqInt)((longAt((void *)(obj - BaseHeaderSize))))
                           << 8)))))) >>
                   8
             : numSlots);
    goto l2;
  }
  assert(!((fmt == (forwardedFormat()))));
  if (fmt < (firstCompiledMethodFormat())) {
    numPointerSlots = 0;
    goto l2;
  }

  /* begin methodHeaderFromSavedFirstField: */
  assert(((((longAt((void *)(firstFieldPtr)))) & 7) == 1));
  header = longAt((void *)(firstFieldPtr));

  /* begin literalCountOfMethodHeader: */
  assert((((header) & 7) == 1));
  numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
  numPointerSlots = numLiterals + LiteralStart;
  /* end numPointerSlotsWhileCompactingOf:withFormat:savedFirstFieldPointer: */
l2:
  if ((fmt <= 5 /* lastPointerFormat */) && (numPointerSlots > 0)) {
    oop = longAt((void *)(firstFieldPtr));
    if (((!(oop & (tagMask())))) &&
        (/* isMobile: */
         (oopisGreaterThanOrEqualToandLessThanOrEqualTo(oop, mobileStart,
                                                        lastMobileObject)) &&
         (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) &
             (1U << (pinnedBitByteShift()))) != 0)))) {
      assert(isMarked(oop));
      fwd = longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
      assert(isPostMobile(fwd));
      longAtput((void *)(firstFieldPtr), fwd);
    }
  }

  /* excludes CompiledMethod
     Relocate the saved first field; Note that CompiledMethods can be excluded
     since their first field is either a SmallInteger or a reference to a
     CogMethod outside of oldSpace. */
  for (i = 1; i < numPointerSlots; i += 1) {
    oop = longAt((void *)((obj + BaseHeaderSize) +
                          ((((usqInt)(i) << (shiftForWord()))))));
    if (((!(oop & (tagMask())))) &&
        (/* isMobile: */
         (oopisGreaterThanOrEqualToandLessThanOrEqualTo(oop, mobileStart,
                                                        lastMobileObject)) &&
         (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) &
             (1U << (pinnedBitByteShift()))) != 0)))) {
      assert((isMarked(oop)) || (obj == (hiddenRootsObject())));
      fwd = longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
      assert(isPostMobile(fwd));

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(obj)) && (!(isForwarded(obj))));
      assert(validStorePointerUncheckedArgs(i, obj, fwd));
      longAtput((void *)((obj + BaseHeaderSize) +
                         ((((usqInt)(i) << (shiftForWord()))))),
                fwd);
    }
  }
}
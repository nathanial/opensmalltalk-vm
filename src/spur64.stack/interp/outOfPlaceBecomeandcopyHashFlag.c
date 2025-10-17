/* Extracted from interp.c:42114 (function outOfPlaceBecomeandcopyHashFlag). */

/*	in an effort to fix a compiler bug with two-way become post r3427 */
/*	Allocate two new objects, n1 & n2. Copy the contents appropriately.
        Convert obj1 and obj2
        into forwarding objects pointing to n2 and n1 respectively. No need to
        check if cloneObject:
        succeeds because an earlier pass over objects ensured that there is
   enough memory.
 */

/* SpurMemoryManager>>#outOfPlaceBecome:and:copyHashFlag: */

static NeverInline void
outOfPlaceBecomeandcopyHashFlag(sqInt obj1, sqInt obj2, sqInt copyHashFlag) {
  sqInt clone1;
  sqInt clone2;
  sqInt hash;

  clone1 = (((longAt((void *)(obj1))) & (classIndexMask())) ==
                    ClassMethodContextCompactIndex
                ? cloneContext(obj1)
                : cloneObject(obj1));
  clone2 = (((longAt((void *)(obj2))) & (classIndexMask())) ==
                    ClassMethodContextCompactIndex
                ? cloneContext(obj2)
                : cloneObject(obj2));
  if (
#if IMMUTABILITY
      ((((usqInt)((
            byteAt((void *)(obj1 + (immutableExtraBitsByteOffset())))))) >>
        (immutableBitByteShift())) &
       1) != 0
#else
      0
#endif
  ) {
    /* begin setIsImmutableOf:to: */
    longAtput((void *)(clone1),
              (longAt((void *)(clone1))) | (1U << (immutableBitShift())));
  }
  if (
#if IMMUTABILITY
      ((((usqInt)((
            byteAt((void *)(obj2 + (immutableExtraBitsByteOffset())))))) >>
        (immutableBitByteShift())) &
       1) != 0
#else
      0
#endif
  ) {
    /* begin setIsImmutableOf:to: */
    longAtput((void *)(clone2),
              (longAt((void *)(clone2))) | (1U << (immutableBitShift())));
  }
  if (copyHashFlag) {
    hash = (long32At((void *)(obj1 + 4))) & (identityHashHalfWordMask());

    /* begin setHashBitsOf:to: */
    long32Atput(
        (void *)(clone1 + 4),
        ((((long32At((void *)(clone1 + 4))) | (identityHashHalfWordMask())) -
          (identityHashHalfWordMask()))) +
            (hash & (identityHashHalfWordMask())));
    hash = (long32At((void *)(obj2 + 4))) & (identityHashHalfWordMask());

    /* begin setHashBitsOf:to: */
    long32Atput(
        (void *)(clone2 + 4),
        ((((long32At((void *)(clone2 + 4))) | (identityHashHalfWordMask())) -
          (identityHashHalfWordMask()))) +
            (hash & (identityHashHalfWordMask())));
  } else {
    hash = (long32At((void *)(obj2 + 4))) & (identityHashHalfWordMask());

    /* begin setHashBitsOf:to: */
    long32Atput(
        (void *)(clone1 + 4),
        ((((long32At((void *)(clone1 + 4))) | (identityHashHalfWordMask())) -
          (identityHashHalfWordMask()))) +
            (hash & (identityHashHalfWordMask())));
    hash = (long32At((void *)(obj1 + 4))) & (identityHashHalfWordMask());

    /* begin setHashBitsOf:to: */
    long32Atput(
        (void *)(clone2 + 4),
        ((((long32At((void *)(clone2 + 4))) | (identityHashHalfWordMask())) -
          (identityHashHalfWordMask()))) +
            (hash & (identityHashHalfWordMask())));
  }
  assert((((isForwardedObjectClassIndexPun()) >= 0) &&
          ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
  assert(
      (((forwardedFormat()) >= 0) && ((forwardedFormat()) <= (formatMask()))));
  longAtput((void *)(obj1),
            ((longAt((void *)(obj1))) &
             (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) +
                             (classIndexMask())))) +
                ((isForwardedObjectClassIndexPun()) +
                 ((((usqInt)((forwardedFormat())) << (formatShift()))))));
#if IMMUTABILITY
  /* begin setIsImmutableOf:to: */
  longAtput((void *)(obj1), (longAt((void *)(obj1))) &
                                (~(usqIntptr_t)(1U << (immutableBitShift()))));
#endif // IMMUTABILITY

  assert(isForwarded(obj1));
  assert(!(isOopForwarded(clone2)));

  /* begin isOldObject: */
  assert(isNonImmediate(obj1));
  if (oopisGreaterThanOrEqualTo(obj1, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(clone2 & (tagMask())))) && (oopisLessThan(clone2, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(obj1 + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(obj1);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((obj1 + BaseHeaderSize) + (0U << (shiftForWord()))),
            clone2);

  /* For safety make sure the forwarder has a slot count that includes its
   * contents. */
  if (!(byteAt((void *)(obj1 + (numSlotsFieldByteOffset()))))) {
    /* rawNumSlotsOf:put: */
    byteAtput((void *)(obj1 + (numSlotsFieldByteOffset())), 1);
  }

  /* begin forward:to: */
  /* begin set:classIndexTo:formatTo: */
  assert((((isForwardedObjectClassIndexPun()) >= 0) &&
          ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
  assert(
      (((forwardedFormat()) >= 0) && ((forwardedFormat()) <= (formatMask()))));
  longAtput((void *)(obj2),
            ((longAt((void *)(obj2))) &
             (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) +
                             (classIndexMask())))) +
                ((isForwardedObjectClassIndexPun()) +
                 ((((usqInt)((forwardedFormat())) << (formatShift()))))));
#if IMMUTABILITY
  /* begin setIsImmutableOf:to: */
  longAtput((void *)(obj2), (longAt((void *)(obj2))) &
                                (~(usqIntptr_t)(1U << (immutableBitShift()))));
#endif // IMMUTABILITY

  assert(isForwarded(obj2));
  assert(!(isOopForwarded(clone1)));

  /* begin isOldObject: */
  assert(isNonImmediate(obj2));
  if (oopisGreaterThanOrEqualTo(obj2, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(clone1 & (tagMask())))) && (oopisLessThan(clone1, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(obj2 + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(obj2);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((obj2 + BaseHeaderSize) + (0U << (shiftForWord()))),
            clone1);

  /* For safety make sure the forwarder has a slot count that includes its
   * contents. */
  if (!(byteAt((void *)(obj2 + (numSlotsFieldByteOffset()))))) {
    /* rawNumSlotsOf:put: */
    byteAtput((void *)(obj2 + (numSlotsFieldByteOffset())), 1);
  }
  if ((((/* begin isYoungObject: */
         assert(isNonImmediate(obj1)), oopisLessThan(obj1, oldSpaceStart))) !=
       ((/* begin isYoungObject: */
         assert(isNonImmediate(clone2)),
         oopisLessThan(clone2, oldSpaceStart)))) ||
      (((/* begin isYoungObject: */
         assert(isNonImmediate(obj2)), oopisLessThan(obj2, oldSpaceStart))) !=
       ((/* begin isYoungObject: */
         assert(isNonImmediate(clone1)),
         oopisLessThan(clone1, oldSpaceStart))))) {
    becomeEffectsFlags = becomeEffectsFlags | OldBecameNewFlag;
  }
}
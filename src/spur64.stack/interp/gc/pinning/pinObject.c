/* Extracted from interp.c:42255 (function pinObject). */

/*	Attempt to pin objOop, which must not be immediate.
        If the attempt succeeds answer objOop's (possibly moved) oop.
        If the attempt fails, which can only occur if there is no memory, answer
        0.  */

/* SpurMemoryManager>>#pinObject: */

sqInt pinObject(sqInt objOop) {
  sqInt i;
  sqInt oldClone;
  SpurSegmentInfo *seg;

  assert(isNonImmediate(objOop));
  assert(!((isForwarded(objOop))));

  /* We choose to clone to keep pinned objects together to reduce fragmentation,
     if the object is not too large, assuming that pinning is rare and that
     fragmentation is a bad thing. Too large is defined as over 1mb.  The size
     of a 640x480x4 bitmap is 1228800. */

  /* begin isOldObject: */
  assert(isNonImmediate(objOop));
  if (oopisGreaterThanOrEqualTo(objOop, oldSpaceStart)) {
    if ((numBytesOf(objOop)) > (0x100000)) {
      setIsPinnedOfto(objOop, 1);
      return objOop;
    }
    seg = segmentContainingObj(objOop);
    if ((seg->containsPinned)) {
      setIsPinnedOfto(objOop, 1);
      return objOop;
    }

    /* begin someSegmentContainsPinned */
    for (i = 0; i < numSegments; i += 1) {
      if (((segments[i]).containsPinned)) {
        goto l1;
      }
    }
    setIsPinnedOfto(objOop, 1);
    (seg->containsPinned = 1);
    return objOop;
  l1:;
  }
  oldClone = cloneInOldSpaceforPinning(objOop, 1);
  if (oldClone) {
    becomeEffectsFlags = becomeEffectFlagsFor(objOop);
    setIsPinnedOfto(oldClone, 1);

    /* begin forward:to: */
    /* begin set:classIndexTo:formatTo: */
    assert((((isForwardedObjectClassIndexPun()) >= 0) &&
            ((isForwardedObjectClassIndexPun()) <= (classIndexMask()))));
    assert((((forwardedFormat()) >= 0) &&
            ((forwardedFormat()) <= (formatMask()))));
    longAtput(
        (void *)(objOop),
        ((longAt((void *)(objOop))) &
         (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) +
                         (classIndexMask())))) +
            ((isForwardedObjectClassIndexPun()) +
             ((((usqInt)((forwardedFormat())) << (formatShift()))))));
#if IMMUTABILITY
    /* begin setIsImmutableOf:to: */
    longAtput((void *)(objOop),
              (longAt((void *)(objOop))) &
                  (~(usqIntptr_t)(1U << (immutableBitShift()))));
#endif // IMMUTABILITY

    assert(isForwarded(objOop));
    assert(!(isOopForwarded(oldClone)));

    /* begin isOldObject: */
    assert(isNonImmediate(objOop));
    if (oopisGreaterThanOrEqualTo(objOop, oldSpaceStart)) {
      if (/* isYoung: */
          ((!(oldClone & (tagMask())))) &&
          (oopisLessThan(oldClone, oldSpaceStart))) {
        /* begin possibleRootStoreInto: */
        if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
              (1U << (rememberedBitByteShift())))) {
          remember(objOop);
        }
      }
    }

    /* most stores into young objects */
    longAtput((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))),
              oldClone);

    /* For safety make sure the forwarder has a slot count that includes its
     * contents. */
    if (!(byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) {
      /* rawNumSlotsOf:put: */
      byteAtput((void *)(objOop + (numSlotsFieldByteOffset())), 1);
    }

    /* begin followSpecialObjectsOop */
    if ((!((longAt((void *)(specialObjectsOop))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      validatedIntegerClassFlags = 0;

      specialObjectsOop = followForwarded(specialObjectsOop);
    }
    followForwardedObjectFieldstoDepth(specialObjectsOop, 0);

    /* begin postBecomeAction: */
    spurPostBecomeAction(becomeEffectsFlags);
    postBecomeScanClassTable(becomeEffectsFlags);
    becomeEffectsFlags = 0;
  }
  return oldClone;
}

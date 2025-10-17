/* Extracted from interp.c:28384 (function isMaybeOldScavengeSurvivor). */

/*	Answer whether the oop has survived a scavenge. This version is
        for processing weak survivors and must cope with the scavenge in
        freeUnmarkedObjectsAndSortAndCoalesceFreeSpaceForPigCompact. */

/* SpurGenerationScavenger>>#isMaybeOldScavengeSurvivor: */

static sqInt isMaybeOldScavengeSurvivor(sqInt oop) {
  sqInt target;

  if (((oop & (tagMask())) != 0)) {
    return 1;
  }
  if ((!((longAt((void *)(oop))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    /* begin followForwarded: */
    assert(isUnambiguouslyForwarder(oop));
    target =
        longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
    while (/* isOopForwarded: */
           isOopForwarded(target)) {
      target = longAt(
          (void *)((target + BaseHeaderSize) + (0U << (shiftForWord()))));
    }
    if (((oop & (tagMask())) != 0)) {
      return 1;
    }
  } else {
    target = oop;
  }
  return (oopisGreaterThanOrEqualTo(target, oldSpaceStart)
              ? (tenureCriterion != MarkOnTenure) ||
                    (((byteAt((void *)(target + (markBitsByteOffset())))) &
                      (1U << (markedBitByteShift()))) != 0)
              : oopisGreaterThanOrEqualToandLessThan(
                    target, ((futureSpace).start), futureSurvivorStart));
}
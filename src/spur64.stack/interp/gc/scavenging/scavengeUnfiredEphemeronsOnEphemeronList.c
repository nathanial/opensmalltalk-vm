/* Extracted from interp.c:29411 (function
 * scavengeUnfiredEphemeronsOnEphemeronList). */

/*	There may be ephemerons to be scavenged on the ephemeronList.
        Scavenge any with unfired (live) keys, removing them from the
        list, and answer if any with unfired keys were found. */

/* SpurGenerationScavenger>>#scavengeUnfiredEphemeronsOnEphemeronList */

static sqInt scavengeUnfiredEphemeronsOnEphemeronList(void) {
  sqInt corpseOffset;
  sqInt ephemeron;
  usqInt ephemeronCorpse;
  sqInt offsetToNextCorpse;
  usqInt previousCorpse;
  sqInt unfiredEphemeronsScavenged;

  previousCorpse = 0;
  if (!ephemeronList) {
    return 0;
  }
  unfiredEphemeronsScavenged = 0;
  corpseOffset = ephemeronList;
  while (corpseOffset != 0) {
    ephemeronCorpse =
        ((((usqInt)((corpseOffset - 1)) << 3 /* shiftForAllocationUnit */))) +
        newSpaceStart;
    assert(isForwarded(ephemeronCorpse));

    ephemeron = followForwarded(ephemeronCorpse);
    offsetToNextCorpse =
        ((((usqInt)(((long32At((void *)(ephemeronCorpse + 4))) &
                     (identityHashHalfWordMask())))
           << (formatFieldWidthShift())))) +
        ((byteAt((void *)(ephemeronCorpse + (formatFieldByteOffset())))) &
         (formatMask()));
    if (isScavengeSurvivor(
            (/* begin keyOfEphemeron: */
             assert((isNonImmediate(ephemeron)) && (isObjEphemeron(ephemeron))),
             /* fetchPointer:ofObject: */
             fetchPointerofObject(0U, ephemeron)))) {
      if (corpseOffset == ephemeronList) {
        ephemeronList = (offsetToNextCorpse ? offsetToNextCorpse : 0);
      } else {
        /* begin setCorpseOffsetOf:to: */
        assert(isYoung(previousCorpse));
        assert(isForwarded(previousCorpse));
        long64Atput(
            (void *)(previousCorpse),
            headerForSlotshashformatclassIndex(
                byteAt((void *)(previousCorpse + (numSlotsFieldByteOffset()))),
                ((usqInt)(offsetToNextCorpse)) >> (formatFieldWidthShift()),
                offsetToNextCorpse & (formatMask()),
                isForwardedObjectClassIndexPun()));
      }
      unfiredEphemeronsScavenged = 1;
      ((void)(scavengeReferentsOf(ephemeron)));
    } else {
      previousCorpse = ephemeronCorpse;
    }
    corpseOffset = offsetToNextCorpse;
  }
  return unfiredEphemeronsScavenged;
}
/* Extracted from interp.c:28533 (function processEphemerons). */

/*	There are ephemerons to be scavenged. Scavenge them and fire any whose
        keys are
        still in pastSpace and/or eden. The unscavenged ephemerons in this cycle
        can only be
        fired if all the unscavenged ephemerons in this cycle are firable,
   because references to ephemeron keys from unfired ephemerons should prevent
   the ephemerons with those keys from firing. So scavenge ephemerons with
   surviving keys, and only if none are found, fire ephemerons with unreferenced
   keys, and scavenge them. Read the class comment for a more in-depth
   description of the algorithm. */

/* SpurGenerationScavenger>>#processEphemerons */

static void processEphemerons(void) {
  sqInt ephemeron;
  usqInt ephemeronCorpse;
  sqInt hasNewReferents;
  sqInt i;
  sqInt key;
  sqInt listOffset;
  usqInt oldCorpse;
  sqInt oldList;
  sqInt referrer;
  sqInt unfiredEphemeronsScavenged;
  sqInt valuePointer;

  /* begin scavengeUnfiredEphemeronsInRememberedSet */
  unfiredEphemeronsScavenged = 0;
  i = 0;
  while (i < numRememberedEphemerons) {
    referrer = rememberedSet[i];
    assert(isEphemeron(referrer));
    if (isScavengeSurvivor(
            (/* begin keyOfEphemeron: */
             assert((isNonImmediate(referrer)) && (isObjEphemeron(referrer))),
             /* fetchPointer:ofObject: */
             longAt((void *)((referrer + BaseHeaderSize) +
                             (0U << (shiftForWord()))))))) {
      unfiredEphemeronsScavenged = 1;
      hasNewReferents = scavengeReferentsOf(referrer);

      /* remove from unscanned ephemerons in set by swapping with last ephemeron
       */
      numRememberedEphemerons -= 1;
      rememberedSet[i] = (rememberedSet[numRememberedEphemerons]);
      if (hasNewReferents) {
        rememberedSet[numRememberedEphemerons] = referrer;
      } else {
        previousRememberedSetSize -= 1;
        rememberedSetSize -= 1;
        rememberedSet[numRememberedEphemerons] =
            (rememberedSet[previousRememberedSetSize]);
        rememberedSet[previousRememberedSetSize] =
            (rememberedSet[rememberedSetSize]);
        setIsRememberedOfto(referrer, 0);
      }
    } else {
      i += 1;
    }
  }
  if (scavengeUnfiredEphemeronsOnEphemeronList()) {
    unfiredEphemeronsScavenged = 1;
  }
  if (!unfiredEphemeronsScavenged) {
    /* begin fireEphemeronsInRememberedSet */
    assert(noUnfiredEphemeronsAtEndOfRememberedSet());
    i = 0;
    while (i < numRememberedEphemerons) {
      ephemeron = rememberedSet[i];
      assert(isEphemeron(ephemeron));

      /* begin keyOfEphemeron: */
      assert((isNonImmediate(ephemeron)) && (isObjEphemeron(ephemeron)));
      key = longAt(
          (void *)((ephemeron + BaseHeaderSize) + (0U << (shiftForWord()))));
      if (!(isScavengeSurvivor(key))) {
        valuePointer = copyAndForward(key);

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(ephemeron)) && (!(isForwarded(ephemeron))));
        assert(validStorePointerUncheckedArgs(0, ephemeron, valuePointer));
        longAtput(
            (void *)((ephemeron + BaseHeaderSize) + (0U << (shiftForWord()))),
            valuePointer);
        fireEphemeron(ephemeron);
      }

      /* Fired ephemerons should have had their format changed. */
      assert(!(((isScavengeSurvivor(key)) && (isEphemeron(ephemeron)))));
      if (scavengeReferentsOf(ephemeron)) {
        i += 1;
      } else {
        setIsRememberedOfto(ephemeron, 0);

        /* remove from set by overwriting with next-to-be scanned */
        numRememberedEphemerons -= 1;
        previousRememberedSetSize -= 1;
        rememberedSetSize -= 1;

        /* First overwrite with last firable ephemeron (could be a noop if this
           is the last one). Then overwrite last firable entry with next
           unscanned rememberedSet entry (could also be a noop). Then overwrite
           next unscanned entry with last unscanned rememberedSet entry (could
           also be a noop). */
        rememberedSet[i] = (rememberedSet[numRememberedEphemerons]);
        rememberedSet[numRememberedEphemerons] =
            (rememberedSet[previousRememberedSetSize]);
        rememberedSet[previousRememberedSetSize] =
            (rememberedSet[rememberedSetSize]);
      }
    }

    /* no more firable ephemerons in this cycle.
       scavengeRememberedSetStartingAt: may find new ones. */
    numRememberedEphemerons = 0;

    /* begin fireEphemeronsOnEphemeronList */
    if (!ephemeronList) {
      goto l1;
    }
    oldCorpse = null;
    ephemeronCorpse =
        ((((usqInt)((ephemeronList - 1)) << 3 /* shiftForAllocationUnit */))) +
        newSpaceStart;

    /* Reset the list head so that new ephemerons will get added
       to a new list, not concatenated on the one we are scanning. */
    oldList = ephemeronList;
    ephemeronList = null;
    while (ephemeronCorpse) {
      assert((isYoung(ephemeronCorpse)) && (isForwarded(ephemeronCorpse)));

      /* begin followForwarded: */
      assert(isUnambiguouslyForwarder(ephemeronCorpse));
      ephemeron = longAt((void *)((ephemeronCorpse + BaseHeaderSize) +
                                  (0U << (shiftForWord()))));
      while (/* isOopForwarded: */
             ((!(ephemeron & (tagMask())))) &&
             ((!((longAt((void *)(ephemeron))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
        ephemeron = longAt(
            (void *)((ephemeron + BaseHeaderSize) + (0U << (shiftForWord()))));
      }

      /* begin keyOfMaybeFiredEphemeron: */
      assert((isNonImmediate(ephemeron)) && (isMaybeFiredEphemeron(ephemeron)));
      key = longAt(
          (void *)((ephemeron + BaseHeaderSize) + (0U << (shiftForWord()))));
      if (!(isScavengeSurvivor(key))) {
        valuePointer = copyAndForward(key);

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(ephemeron)) && (!(isForwarded(ephemeron))));
        assert(validStorePointerUncheckedArgs(0, ephemeron, valuePointer));
        longAtput(
            (void *)((ephemeron + BaseHeaderSize) + (0U << (shiftForWord()))),
            valuePointer);
        fireEphemeron(ephemeron);
      }

      /* Fired ephemerons should have had their format changed. */
      assert(!(((isScavengeSurvivor(key)) && (isEphemeron(ephemeron)))));
      ((void)(scavengeReferentsOf(ephemeron)));
      oldCorpse = ephemeronCorpse;

      /* begin nextCorpseOrNil: */
      assert(isYoung(ephemeronCorpse));
      listOffset =
          ((((usqInt)(((long32At((void *)(ephemeronCorpse + 4))) &
                       (identityHashHalfWordMask())))
             << (formatFieldWidthShift())))) +
          ((byteAt((void *)(ephemeronCorpse + (formatFieldByteOffset())))) &
           (formatMask()));
      ephemeronCorpse =
          ((sqInt)((listOffset ? ((((usqInt)((listOffset - 1))
                                    << 3 /* shiftForAllocationUnit */))) +
                                     newSpaceStart
                               : 0)));
    }
    /* end fireEphemeronsOnEphemeronList */
  l1:;
  }
}
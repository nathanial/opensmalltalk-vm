/* Extracted from interp.c:29028 (function scavengeReferentsOf). */

/*	scavengeReferentsOf: referrer inspects relevant pointers in referrer. If
        any are new objects, it has them moved to FutureSurvivorSpace, and
        answers truth. If there are no new referents, it answers falsity. To
        handle weak arrays, if the referrer is weak only scavenge strong slots
   and answer true so that it won't be removed from the remembered set until
        later. To handle ephemerons, only scavenge other slots if the key slot
   has already been scavenged. */
/*	forwarding objects should be followed by callers,
        unless the forwarder is a root in the remembered table. */

/* SpurGenerationScavenger>>#scavengeReferentsOf: */

static sqInt scavengeReferentsOf(sqInt referrer) {
  sqInt foundNewReferent;
  sqInt i;
  usqInt numSlots;
  sqInt referent;
  sqInt referentSqInt;
  sqInt toDoLimit;

  assert((!(isForwarded(referrer))) || (isRemembered(referrer)));
  switch ((byteAt((void *)(referrer + (formatFieldByteOffset())))) &
          (formatMask())) {
  case nonIndexablePointerFormat():
  case arrayFormat():
    /* begin scavengeReferentsOfPointers: */
    foundNewReferent = 0;
    toDoLimit =
        ((/* begin numSlotsOf: */
          assert((classIndexOf(referrer)) > (isForwardedObjectClassIndexPun())),
          (((numSlots =
                 byteAt((void *)(referrer + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                          (void *)(referrer - BaseHeaderSize))))
                                      << 8)))))) >>
                     8
               : numSlots))) -
        1;
    for (i = 0; i <= toDoLimit; i += 1) {
      referent = longAt((void *)((referrer + BaseHeaderSize) +
                                 ((((usqInt)(i) << (shiftForWord()))))));
      if ((!(referent & (tagMask())))) {
        if ((!((longAt((void *)(referent))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          /* begin followForwarded: */
          assert(isUnambiguouslyForwarder(referent));
          referentSqInt = longAt(
              (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
          while (isOopForwarded(referentSqInt)) {
            referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) +
                                            (0U << (shiftForWord()))));
          }
          referent = referentSqInt;
        }

        /* begin isReallyYoungObject: */
        /* begin isYoungObject: */
        assert(isNonImmediate(referent));
        if (oopisLessThan(referent, oldSpaceStart)) {
          if (oopisGreaterThanOrEqualToandLessThan(
                  referent, ((futureSpace).start), futureSurvivorStart)) {
            foundNewReferent = 1;
          } else {
            referent = copyAndForward(referent);

            /* begin isReallyYoungObject: */
            /* begin isYoungObject: */
            assert(isNonImmediate(referent));
            if (oopisLessThan(referent, oldSpaceStart)) {
              foundNewReferent = 1;
            }
          }
        }

        /* if target is already in future space forwarding pointer was due to a
         * become:. */

        /* storePointerUnchecked:ofMaybeForwardedObject:withValue: */
        longAtput((void *)((referrer + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  referent);
      }
    }
    return foundNewReferent;

  case indexablePointersFormat():
    /* begin scavengeReferentsOfIndexablePointers: */
    foundNewReferent = 0;
    toDoLimit = (numSlotsOfIndexablePointerObj(referrer)) - 1;
    for (i = 0; i <= toDoLimit; i += 1) {
      referent = longAt((void *)((referrer + BaseHeaderSize) +
                                 ((((usqInt)(i) << (shiftForWord()))))));
      if ((!(referent & (tagMask())))) {
        if ((!((longAt((void *)(referent))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          /* begin followForwarded: */
          assert(isUnambiguouslyForwarder(referent));
          referentSqInt = longAt(
              (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
          while (isOopForwarded(referentSqInt)) {
            referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) +
                                            (0U << (shiftForWord()))));
          }
          referent = referentSqInt;
        }

        /* begin isReallyYoungObject: */
        /* begin isYoungObject: */
        assert(isNonImmediate(referent));
        if (oopisLessThan(referent, oldSpaceStart)) {
          if (oopisGreaterThanOrEqualToandLessThan(
                  referent, ((futureSpace).start), futureSurvivorStart)) {
            foundNewReferent = 1;
          } else {
            referent = copyAndForward(referent);

            /* begin isReallyYoungObject: */
            /* begin isYoungObject: */
            assert(isNonImmediate(referent));
            if (oopisLessThan(referent, oldSpaceStart)) {
              foundNewReferent = 1;
            }
          }
        }

        /* if target is already in future space forwarding pointer was due to a
         * become:. */

        /* storePointerUnchecked:ofMaybeForwardedObject:withValue: */
        longAtput((void *)((referrer + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  referent);
      }
    }
    return foundNewReferent;

  case weakArrayFormat():
    /* begin scavengeReferentsOfWeakling: */
    toDoLimit = (numStrongSlotsOfWeakling(referrer)) - 1;
    for (i = 0; i <= toDoLimit; i += 1) {
      referent = longAt((void *)((referrer + BaseHeaderSize) +
                                 ((((usqInt)(i) << (shiftForWord()))))));
      if ((!(referent & (tagMask())))) {
        if ((!((longAt((void *)(referent))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          /* begin followForwarded: */
          assert(isUnambiguouslyForwarder(referent));
          referentSqInt = longAt(
              (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
          while (isOopForwarded(referentSqInt)) {
            referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) +
                                            (0U << (shiftForWord()))));
          }
          referent = referentSqInt;
        }

        /* begin isReallyYoungObject: */
        /* begin isYoungObject: */
        assert(isNonImmediate(referent));
        if (oopisLessThan(referent, oldSpaceStart)) {
          if (!(oopisGreaterThanOrEqualToandLessThan(
                  referent, ((futureSpace).start), futureSurvivorStart))) {
            referent = copyAndForward(referent);
          }
        }

        /* if target is already in future space forwarding pointer was due to a
         * become:. */

        /* storePointerUnchecked:ofMaybeForwardedObject:withValue: */
        longAtput((void *)((referrer + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  referent);
      }
    }

    /* Answer true to keep any weak objects in the remembered set until later.
     */
    return 1;

  case ephemeronFormat():
    if (!(isScavengeSurvivor(
            (/* begin keyOfEphemeron: */
             assert((isNonImmediate(referrer)) && (isObjEphemeron(referrer))),
             /* fetchPointer:ofObject: */
             longAt((void *)((referrer + BaseHeaderSize) +
                             (0U << (shiftForWord())))))))) {
      return 1;
    }

    /* begin scavengeReferentsOfPointers: */
    foundNewReferent = 0;
    toDoLimit =
        ((/* begin numSlotsOf: */
          assert((classIndexOf(referrer)) > (isForwardedObjectClassIndexPun())),
          (((numSlots =
                 byteAt((void *)(referrer + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                          (void *)(referrer - BaseHeaderSize))))
                                      << 8)))))) >>
                     8
               : numSlots))) -
        1;
    for (i = 0; i <= toDoLimit; i += 1) {
      referent = longAt((void *)((referrer + BaseHeaderSize) +
                                 ((((usqInt)(i) << (shiftForWord()))))));
      if ((!(referent & (tagMask())))) {
        if ((!((longAt((void *)(referent))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          /* begin followForwarded: */
          assert(isUnambiguouslyForwarder(referent));
          referentSqInt = longAt(
              (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
          while (isOopForwarded(referentSqInt)) {
            referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) +
                                            (0U << (shiftForWord()))));
          }
          referent = referentSqInt;
        }

        /* begin isReallyYoungObject: */
        /* begin isYoungObject: */
        assert(isNonImmediate(referent));
        if (oopisLessThan(referent, oldSpaceStart)) {
          if (oopisGreaterThanOrEqualToandLessThan(
                  referent, ((futureSpace).start), futureSurvivorStart)) {
            foundNewReferent = 1;
          } else {
            referent = copyAndForward(referent);

            /* begin isReallyYoungObject: */
            /* begin isYoungObject: */
            assert(isNonImmediate(referent));
            if (oopisLessThan(referent, oldSpaceStart)) {
              foundNewReferent = 1;
            }
          }
        }

        /* if target is already in future space forwarding pointer was due to a
         * become:. */

        /* storePointerUnchecked:ofMaybeForwardedObject:withValue: */
        longAtput((void *)((referrer + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  referent);
      }
    }
    return foundNewReferent;

  case forwardedFormat():
    /* begin scavengeReferentOfForwarder: */
    foundNewReferent = 0;
    referent = longAt(
        (void *)((referrer + BaseHeaderSize) + (0U << (shiftForWord()))));
    if ((!(referent & (tagMask())))) {
      if ((!((longAt((void *)(referent))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        /* begin followForwarded: */
        assert(isUnambiguouslyForwarder(referent));
        referentSqInt = longAt(
            (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
        while (isOopForwarded(referentSqInt)) {
          referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) +
                                          (0U << (shiftForWord()))));
        }
        referent = referentSqInt;
      }

      /* begin isReallyYoungObject: */
      /* begin isYoungObject: */
      assert(isNonImmediate(referent));
      if (oopisLessThan(referent, oldSpaceStart)) {
        if (oopisGreaterThanOrEqualToandLessThan(
                referent, ((futureSpace).start), futureSurvivorStart)) {
          foundNewReferent = 1;
        } else {
          referent = copyAndForward(referent);

          /* begin isReallyYoungObject: */
          /* begin isYoungObject: */
          assert(isNonImmediate(referent));
          if (oopisLessThan(referent, oldSpaceStart)) {
            foundNewReferent = 1;
          }
        }
      }

      /* if target is already in future space forwarding pointer was due to a
       * become:. */

      /* storePointerUnchecked:ofMaybeForwardedObject:withValue: */
      longAtput(
          (void *)((referrer + BaseHeaderSize) + (0U << (shiftForWord()))),
          referent);
    }

    /* a forwarding pointer could be because of become: or scavenging. */
    return foundNewReferent;

  case firstCompiledMethodFormat():
  case (firstCompiledMethodFormat()) + 1:
  case (firstCompiledMethodFormat()) + 2:
  case (firstCompiledMethodFormat()) + 3:
  case (firstCompiledMethodFormat()) + 4:
  case (firstCompiledMethodFormat()) + 5:
  case (firstCompiledMethodFormat()) + 6:
  case (firstCompiledMethodFormat()) + 7:
    /* begin scavengeReferentsOfCompiledCode: */
    foundNewReferent = 0;
    toDoLimit = literalCountOf(referrer);
    for (i = 1; i <= toDoLimit; i += 1) {
      referent = longAt((void *)((referrer + BaseHeaderSize) +
                                 ((((usqInt)(i) << (shiftForWord()))))));
      if ((!(referent & (tagMask())))) {
        if ((!((longAt((void *)(referent))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          /* begin followForwarded: */
          assert(isUnambiguouslyForwarder(referent));
          referentSqInt = longAt(
              (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
          while (isOopForwarded(referentSqInt)) {
            referentSqInt = longAt((void *)((referentSqInt + BaseHeaderSize) +
                                            (0U << (shiftForWord()))));
          }
          referent = referentSqInt;
        }

        /* begin isReallyYoungObject: */
        /* begin isYoungObject: */
        assert(isNonImmediate(referent));
        if (oopisLessThan(referent, oldSpaceStart)) {
          if (oopisGreaterThanOrEqualToandLessThan(
                  referent, ((futureSpace).start), futureSurvivorStart)) {
            foundNewReferent = 1;
          } else {
            referent = copyAndForward(referent);

            /* begin isReallyYoungObject: */
            /* begin isYoungObject: */
            assert(isNonImmediate(referent));
            if (oopisLessThan(referent, oldSpaceStart)) {
              foundNewReferent = 1;
            }
          }
        }

        /* if target is already in future space forwarding pointer was due to a
         * become:. */

        /* storePointerUnchecked:ofMaybeForwardedObject:withValue: */
        longAtput((void *)((referrer + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  referent);
      }
    }
    return foundNewReferent;

  default:
    assert(((numSlotsOf(referrer)) == 0) || (isWordsOrBytesNonImm(referrer)));
  }

  /* Scavenge the referents of an ephemeron. If the key
     has not yet been scavenged, don't scavenge it yet. */
  return 0;
}
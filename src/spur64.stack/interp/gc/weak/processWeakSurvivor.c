/* Extracted from interp.c:28771 (function processWeakSurvivor). */

/*	Process a weak survivor on the weakList. Those of its fields
        which have not survived the scavenge should be nilled, and if any
        are, the coInterpreter should be informed via fireFinalization:.
        Answer if the weakObj has any young referents. */

/* SpurGenerationScavenger>>#processWeakSurvivor: */

static sqInt processWeakSurvivor(sqInt weakObj) {
  sqInt classFormat;
  sqInt classPointer;
  sqInt hasYoungReferents;
  sqInt i;
  sqInt numStrongSlots;
  sqInt referent;
  sqInt toDoLimit;
  sqInt weakObjShouldMourn;

  weakObjShouldMourn = (hasYoungReferents = 0);

  /* N.B. generateToByDoLimitExpression:negative:on: guards against (unsigned)0
   * - 1 going +ve */

  /* begin numFixedSlotsOf: */
  classPointer = fetchClassOfNonImm(weakObj);
  classFormat =
      ((fetchPointerofObject(InstanceSpecificationIndex, classPointer)) >> 3);
  numStrongSlots = classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
  for (i = 0; i < numStrongSlots; i += 1) {
    referent = fetchPointerofObject(i, weakObj);
    if (((!(referent & (tagMask())))) &&
        ((/* begin isYoungObject: */
          assert(isNonImmediate(referent)),
          oopisLessThan(referent, oldSpaceStart)))) {
      hasYoungReferents = 1;
    }
  }
  toDoLimit = ((numSlotsOf(weakObj))) - 1;
  for (i = numStrongSlots; i <= toDoLimit; i += 1) {
    referent = fetchPointerofObject(i, weakObj);

    /* Referent could be forwarded due to scavenging or a become:, don't assume.
     */
    if ((!(referent & (tagMask())))) {
      if ((!((longAt((void *)(referent))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        referent = followForwarded(referent);

        /* weakObj is either young or already in remembered table; no need to
         * check */
        assert((isReallyYoungObject(weakObj)) || (isRemembered(weakObj)));

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(weakObj)) && (!(isForwarded(weakObj))));
        assert(validStorePointerUncheckedArgs(i, weakObj, referent));
        longAtput((void *)((weakObj + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  referent);
      }
      if (isMaybeOldScavengeSurvivor(referent)) {
        /* begin isYoungObject: */
        assert(isNonImmediate(referent));
        if (oopisLessThan(referent, oldSpaceStart)) {
          hasYoungReferents = 1;
        }
      } else {
        weakObjShouldMourn = 1;

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(weakObj)) && (!(isForwarded(weakObj))));
        assert(validStorePointerUncheckedArgs(i, weakObj, nilObj));
        longAtput((void *)((weakObj + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  nilObj);
      }
    }
  }
  if (weakObjShouldMourn) {
    /* begin fireFinalization: */
    if (newFinalization) {
      queueMourner(weakObj);
    }

    /* begin signalFinalization: */
    forceInterruptCheck();
    pendingFinalizationSignals += 1;
  }
  return hasYoungReferents;
}

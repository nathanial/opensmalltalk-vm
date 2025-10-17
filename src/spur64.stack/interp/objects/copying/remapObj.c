/* Extracted from interp.c:47945 (function remapObj). */

/*	Scavenge or simply follow objOop. Answer the new location of objOop.
        The send should have been guarded by a send of shouldRemapOop:.
        The method is called remapObj: for compatibility with ObjectMemory. */

/* SpurPlanningCompactor>>#remapObj: */

sqInt remapObj(sqInt objOop) {
  sqInt resolvedObj;

  /* begin slidingCompactionRemapObj: */
  assert(shouldRemapOop(objOop));
  if ((!((longAt((void *)(objOop))) &
         ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
    /* begin followForwarded: */
    assert(isUnambiguouslyForwarder(objOop));
    resolvedObj =
        longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
    while (/* isOopForwarded: */
           ((!(resolvedObj & (tagMask())))) &&
           ((!((longAt((void *)(resolvedObj))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
      resolvedObj = longAt(
          (void *)((resolvedObj + BaseHeaderSize) + (0U << (shiftForWord()))));
    }
  } else {
    assert(!((isInFutureSpace(objOop))));
    resolvedObj = objOop;
  }
  if (gcPhaseInProgress > 0) {
    if (gcPhaseInProgress == ScavengeInProgress) {
      if ((/* isReallyYoung: */
           ((!(resolvedObj & (tagMask())))) &&
           ((/* begin isReallyYoungObject: */ /* begin isYoungObject: */
             assert(isNonImmediate(resolvedObj)),
             oopisLessThan(resolvedObj, oldSpaceStart)))) &&
          (!(oopisGreaterThanOrEqualToandLessThan(
              resolvedObj, ((futureSpace).start), futureSurvivorStart)))) {
        return copyAndForward(resolvedObj);
      }
    } else {
      assert(slidingCompactionInProgress());
      if (/* isMobile: */
          (oopisGreaterThanOrEqualToandLessThanOrEqualTo(objOop, mobileStart,
                                                         lastMobileObject)) &&
          (!(((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
              (1U << (pinnedBitByteShift()))) != 0))) {
        return longAt(
            (void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
      }
    }
  }
  return resolvedObj;
}
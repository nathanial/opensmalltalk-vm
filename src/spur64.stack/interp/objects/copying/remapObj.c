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
    resolvedObj = followForwarded(objOop);
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
        return fetchPointerofObject(0U, objOop);
      }
    }
  }
  return resolvedObj;
}
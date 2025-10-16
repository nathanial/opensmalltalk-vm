/* Extracted from interp.c:28947 (function scavengeLoop). */

/*	This is the inner loop of the main routine, scavenge. It first scavenges
        the new objects immediately
        reachable from old ones. Then it scavenges those that are transitively
        reachable. If this results in a
        promotion, the promotee gets remembered, and it first scavenges objects
        adjacent to the promotee,
        then scavenges the ones reachable from the promoted. This loop continues
        until no more reachable
        objects are left. At that point, pastSurvivorSpace is exchanged with
        futureSurvivorSpace.
        Notice that each pointer in a live object is inspected once and only
   once. The previousRememberedSetSize and previousFutureSurvivorSpaceLimit
   variables ensure that no object is scanned twice, as well as detecting
   closure. If this were not true, some pointers might get forwarded twice.

        An extension of the algorithm presented in David's original paper is to
        handle weak arrays and ephemerons.
        Weak arrays should not have their weak referents scavenged unless there
        are strong references to them.
        Ephemerons should fire if their key is not reachable other than from
        ephemerons and weak arrays.
        Handle this by maintaining a list for weak arrays and a list for
        ephemerons, which allow scavenging these
        objects once all other objects in new space have been scavenged, hence
        allowing the scavenger to
        detect which referents in new space of weak arrays are dead and of
        ephemeron keys are only live due to
        ephemerons. Read the class comment for a more in-depth description of
   the algorithm.  */

/* SpurGenerationScavenger>>#scavengeLoop */

static void scavengeLoop(void) {
  sqInt firstTime;
  sqInt i;
  sqInt previousFutureSurvivorStart;
  StackPage *thePage;

  assert(futureSurvivorStart == ((futureSpace.start)));
  weakList = (ephemeronList = null);
  numRememberedEphemerons = 0;
  firstTime = 1;
  previousRememberedSetSize = 0;
  previousFutureSurvivorStart = futureSurvivorStart;

  /* begin initStackPageGC */
  if (stackPage) {
    externalWriteBackHeadFramePointers();
  }
  for (i = 0; i < numStackPages; i += 1) {
    /* begin stackPageAt: */
    thePage = stackPageAtpages(i, pages);
    (thePage->trace = StackPageUnreached);
  }
  while (1) {
    scavengeRememberedSetStartingAt(previousRememberedSetSize);
    previousRememberedSetSize = rememberedSetSize;
    if (firstTime) {
      mapInterpreterOops();
      mapMournQueue();
      mapExtraRoots();
      firstTime = 0;
    }

    /* if nothing more copied and forwarded (or remembered by
       mapInterpreterOops) to scavenge, and no ephemerons to process, scavenge
       is done. */
    if ((previousRememberedSetSize == rememberedSetSize) &&
        ((previousFutureSurvivorStart == futureSurvivorStart) &&
         ((numRememberedEphemerons == 0) && (!ephemeronList)))) {
      return;
    }
    scavengeFutureSurvivorSpaceStartingAt(previousFutureSurvivorStart);
    previousFutureSurvivorStart = futureSurvivorStart;

    /* no more roots created to scavenge... */
    if (previousRememberedSetSize == rememberedSetSize) {
      if ((numRememberedEphemerons == 0) && (!ephemeronList)) {
        return;
      }
      processEphemerons();
    }
  }
}
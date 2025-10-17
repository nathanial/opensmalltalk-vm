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
	Notice that each pointer in a live object is inspected once and only once.
	The previousRememberedSetSize
	and previousFutureSurvivorSpaceLimit variables ensure that no object is
	scanned twice, as well as
	detecting closure. If this were not true, some pointers might get
	forwarded twice.
	
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
	ephemerons. Read the class comment for a more in-depth description of the
	algorithm.  */

	/* SpurGenerationScavenger>>#scavengeLoop */

static void
scavengeLoop(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt firstTime;
    sqInt i;
    sqInt previousFutureSurvivorStart;
    StackPage *thePage;

	assert(GIV(futureSurvivorStart) == ((GIV(futureSpace).start)));
	GIV(weakList) = (GIV(ephemeronList) = null);
	GIV(numRememberedEphemerons) = 0;
	firstTime = 1;
	GIV(previousRememberedSetSize) = 0;
	previousFutureSurvivorStart = GIV(futureSurvivorStart);

	/* begin initStackPageGC */
	if (GIV(stackPage)) {
		/* begin externalWriteBackHeadFramePointers */
		assert((GIV(framePointer) - GIV(stackPointer)) < (LargeContextSlots * BytesPerOop));
		assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
		assert(!((isFree(GIV(stackPage)))));

		/* begin setHeadFP:andSP:inPage: */
		assert(GIV(stackPointer) < GIV(framePointer));
		assert((GIV(stackPointer) < ((GIV(stackPage)->baseAddress)))
		 && (GIV(stackPointer) > (((GIV(stackPage)->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
		assert((GIV(framePointer) < ((GIV(stackPage)->baseAddress)))
		 && (GIV(framePointer) > (((GIV(stackPage)->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
		(GIV(stackPage)->headFP = GIV(framePointer));
		(GIV(stackPage)->headSP = GIV(stackPointer));
		assert(pageListIsWellFormed());
	}
	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		thePage = stackPageAtpages(i, GIV(pages));
		(thePage->trace = StackPageUnreached);
	}
	while (1) {
		scavengeRememberedSetStartingAt(GIV(previousRememberedSetSize));
		GIV(previousRememberedSetSize) = GIV(rememberedSetSize);
		if (firstTime) {
			mapInterpreterOops();
			mapMournQueue();
			mapExtraRoots();
			firstTime = 0;
		}

		/* if nothing more copied and forwarded (or remembered by mapInterpreterOops)
		   to scavenge, and no ephemerons to process, scavenge is done. */
		if ((GIV(previousRememberedSetSize) == GIV(rememberedSetSize))
		 && ((previousFutureSurvivorStart == GIV(futureSurvivorStart))
		 && ((GIV(numRememberedEphemerons) == 0)
		 && (!GIV(ephemeronList))))) {
			return;
		}
		scavengeFutureSurvivorSpaceStartingAt(previousFutureSurvivorStart);
		previousFutureSurvivorStart = GIV(futureSurvivorStart);

		/* no more roots created to scavenge... */
		if (GIV(previousRememberedSetSize) == GIV(rememberedSetSize)) {
			if ((GIV(numRememberedEphemerons) == 0)
			 && (!GIV(ephemeronList))) {
				return;
			}
			processEphemerons();
		}
	}
}
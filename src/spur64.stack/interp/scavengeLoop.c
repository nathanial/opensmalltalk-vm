/* Extracted from interp.c:28947 (function scavengeLoop). */

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
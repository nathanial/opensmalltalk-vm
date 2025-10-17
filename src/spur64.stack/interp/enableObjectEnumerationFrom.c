/* Extracted from interp.c:34598 (function enableObjectEnumerationFrom). */

/*	We use bridges to stitch segments together to make it appear that the heap
	is one contiguous space.
	Bridges at the end of oldSpace segments are maintained. Bridges at the end
	of pastSpace and eden
	are temporary, and are established here, depending on the current sizes of
	pastSpace end eden.
	
	N.B. this introduces complications. Either or both pastSpace and eden may
	be empty, so the 
	bridge from pastSpace may skip eden. pastSpace may be full, so there may
	be no bridge at
	the end of pastSpace. Most difficult, pastSpace could be one 64-bit word
	short of full, but normal
	bridges are two word objects. To make this work we introduce a hack,
	objectAfterMaybeSlimBridge:limit:, which uses a fake overflow slot count
	to get to the start of the next object, which is either one or two
	words away, depending on whether the first object in eden has a normal or
	an overflow header. */

	/* SpurMemoryManager>>#enableObjectEnumerationFrom: */

static NoDbgRegParms void
enableObjectEnumerationFrom(sqInt initialObject)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (oopisLessThan(initialObject, GIV(oldSpaceStart))) {
		if (GIV(freeStart) > (((GIV(eden)).start))) {
			/* begin bridgeEdenAndOldSpace */
			initSegmentBridgeWithBytesat(GIV(oldSpaceStart) - GIV(freeStart), GIV(freeStart));
			if (GIV(pastSpaceStart) < (((GIV(eden)).start))) {
				if ((GIV(pastSpaceStart) + BaseHeaderSize) == (((GIV(eden)).start))) {
					hackSlimBridgeToat(objectStartingAt(((GIV(eden)).start)), GIV(pastSpaceStart));

					/* And carefully check the assumption */
					assert((objectAfterMaybeSlimBridgelimit(objectInPastSpaceBefore(GIV(pastSpaceStart)), GIV(nilObj))) == (objectStartingAt(((GIV(eden)).start))));
				}
				else {
					initSegmentBridgeWithBytesat((((GIV(eden)).start)) - GIV(pastSpaceStart), GIV(pastSpaceStart));
				}
			}
		}
		else {
			/* begin bridgePastSpaceAndOldSpace */
			initSegmentBridgeWithBytesat(GIV(oldSpaceStart) - GIV(pastSpaceStart), GIV(pastSpaceStart));
		}
	}
}
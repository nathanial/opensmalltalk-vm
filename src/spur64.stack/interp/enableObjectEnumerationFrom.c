/* Extracted from interp.c:34598 (function enableObjectEnumerationFrom). */

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
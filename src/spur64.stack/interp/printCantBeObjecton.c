/* Extracted from interp.c:42665 (function printCantBeObjecton). */

	/* SpurMemoryManager>>#printCantBeObject:on: */

static NoDbgRegParms void
printCantBeObjecton(sqInt oop, FILE *aStream)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (!(oop & 7 /* (allocationUnit - 1) */)) {
		if ((/* isInNewSpace: */
			(oopisLessThan(oop, GIV(oldSpaceStart)))
		 && (oopisGreaterThanOrEqualTo(oop, GIV(newSpaceStart))))
		 && ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			printForwarderon(oop, aStream);
			return;
		}
		if (oop == (bridgeAt(GIV(numSegments) - 1))) {
			printBridgeon(oop, aStream);
			return;
		}
	}
	fprintf(aStream,
			"%p%s\n",
			((void *)oop),
			(oop & 7 /* (allocationUnit - 1) */
			? " is misaligned"
			: whereIs(oop)));
}
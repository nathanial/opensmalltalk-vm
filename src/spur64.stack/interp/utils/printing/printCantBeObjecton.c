/* Extracted from interp.c:42665 (function printCantBeObjecton). */

/* SpurMemoryManager>>#printCantBeObject:on: */

static void printCantBeObjecton(sqInt oop, FILE *aStream) {
  if (!(oop & 7 /* (allocationUnit - 1) */)) {
    if ((/* isInNewSpace: */
         (oopisLessThan(oop, oldSpaceStart)) &&
         (oopisGreaterThanOrEqualTo(oop, newSpaceStart))) &&
        ((!((longAt((void *)(oop))) &
            ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
      printForwarderon(oop, aStream);
      return;
    }
    if (oop == (bridgeAt(numSegments - 1))) {
      printBridgeon(oop, aStream);
      return;
    }
  }
  fprintf(aStream, "%p%s\n", ((void *)oop),
          (oop & 7 /* (allocationUnit - 1) */
               ? " is misaligned"
               : whereIs(oop)));
}
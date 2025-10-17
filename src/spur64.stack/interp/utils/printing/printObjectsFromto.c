/* Extracted from interp.c:43950 (function printObjectsFromto). */

/*	useful for VM debugging */

/* SpurMemoryManager>>#printObjectsFrom:to: */

void printObjectsFromto(sqInt startAddress, sqInt endAddress) {
  sqInt oop;

  oop = objectBefore(startAddress);
  oop = (oop ? ((objectAfter(oop)) == startAddress ? startAddress : oop)
             : startAddress);
  while (oopisLessThan(oop, endAddress)) {
    if (!((((longAt((void *)(oop))) & (classIndexMask())) ==
           (isFreeObjectClassIndexPun())) ||
          (((longAt((void *)(oop))) & (classIndexMask())) ==
           (segmentBridgePun())))) {
      printOop(oop);
    }
    oop = objectAfter(oop);
  }
}
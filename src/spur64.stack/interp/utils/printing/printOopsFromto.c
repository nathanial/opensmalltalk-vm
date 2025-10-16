/* Extracted from interp.c:44316 (function printOopsFromto). */

/*	useful for VM debugging */

/* SpurMemoryManager>>#printOopsFrom:to: */

void printOopsFromto(sqInt startAddress, sqInt endAddress) {
  sqInt firstNonEntity;
  sqInt inEmptySpace;
  sqInt lastNonEntity;
  sqInt limit;
  sqInt oop;

  firstNonEntity = 0;
  oop = objectBefore(startAddress);
  limit =
      (((((usqIntptr_t)endAddress)) < endOfMemory) ? (((usqIntptr_t)endAddress))
                                                   : endOfMemory);
  oop = (oop ? ((objectAfter(oop)) == startAddress ? startAddress : oop)
             : startAddress);
  inEmptySpace = 0;
  while (oopisLessThan(oop, limit)) {
    printEntity(oop);
    while (1) {
      oop = objectAfter(oop);
      if (!((long64At((void *)(oop))) == 0))
        break;
      if (!inEmptySpace) {
        inEmptySpace = 1;
        firstNonEntity = oop;
      }
      lastNonEntity = oop;
    }
    if (inEmptySpace) {
      inEmptySpace = 0;
      print("skipped empty space from ");

      /* printHexnp: */
      fprintf(transcript, "%p", ((void *)firstNonEntity));
      print(" to ");

      /* printHexnp: */
      fprintf(transcript, "%p", ((void *)lastNonEntity));
      cr();
      oop = objectStartingAt(oop);
    }
  }
}
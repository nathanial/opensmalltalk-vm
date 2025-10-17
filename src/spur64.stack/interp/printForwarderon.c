/* Extracted from interp.c:43060 (function printForwarderon). */

/* SpurMemoryManager>>#printForwarder:on: */

static void printForwarderon(sqInt oop, FILE *aStream) {
  fprintf(
      aStream,
      "%p is a forwarded hdr%d slot size %" PRIuSQINT " object to %p\n",
      ((void *)oop),
      ((byteAt((void *)(oop + (numSlotsFieldByteOffset())))) == (numSlotsMask())
           ? 16
           : 8),
      numSlotsOfAny(oop), ((void *)(followForwarded(oop))));
}
/* Extracted from interp.c:36031 (function goodContextSize). */

/* SpurMemoryManager>>#goodContextSize: */

static sqInt goodContextSize(sqInt oop) {
  usqInt numSlots;
  usqInt numSlotsUsqInt;

  
  numSlots =
      numSlotsOf(oop);
  return (numSlots == SmallContextSlots) || (numSlots == LargeContextSlots);
}
/* Extracted from interp.c:37224 (function isCopiedIntoSegment). */

/*	This is part of storeImageSegmentInto:outPointers:roots:. */

/* SpurMemoryManager>>#isCopiedIntoSegment: */

static int isCopiedIntoSegment(sqInt anObjectInTheHeap) {
  return ((byteAt((void *)(anObjectInTheHeap + (markBitsByteOffset())))) &
          (1U << (markedBitByteShift()))) != 0;
}
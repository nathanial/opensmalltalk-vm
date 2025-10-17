/* Extracted from interp.c:43448 (function printImmediateObjecton). */

/* SpurMemoryManager>>#printImmediateObject:on: */

static void printImmediateObjecton(sqInt oop, FILE *aStream) {
  assert(isImmediate(oop));
  if ((((oop) & 7) == 1)) {
    fprintf(aStream, "%p=%" PRIdSQINT "\n", ((void *)oop),
            ((sqInt)((oop >> 3))));
  }
  if (((oop & (characterTag())) != 0)) {
    fprintf(aStream, "%p=$%" PRIdSQINT " ($%lc)\n", ((void *)oop),
            ((long)(((((usqInt)oop))) >> (numTagBits()))),
            ((wint_t)(((((usqInt)oop))) >> (numTagBits()))));
  }
  if (((oop & (smallFloatTag())) != 0)) {
    fprintf(aStream, "%p=%g\n", ((void *)oop), floatValueOf(oop));
  }
}
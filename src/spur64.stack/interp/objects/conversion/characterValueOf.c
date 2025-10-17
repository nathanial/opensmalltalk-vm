/* Extracted from interp.c:32020 (function characterValueOf). */

/*	Immediate characters are unsigned */

/* SpurMemoryManager>>#characterValueOf: */

usqInt characterValueOf(sqInt oop) {
  return ((((usqInt)oop))) >> (numTagBits());
}
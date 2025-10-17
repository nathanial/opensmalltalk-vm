/* Extracted from interp.c:37168 (function isCharacterObject). */

/* SpurMemoryManager>>#isCharacterObject: */

int isCharacterObject(sqInt oop) { return ((oop & (characterTag())) != 0); }
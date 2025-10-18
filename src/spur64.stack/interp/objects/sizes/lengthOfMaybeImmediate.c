/* Extracted from interp.c:38124 (function lengthOfMaybeImmediate). */

/*	for the message send breakpoint; selectors can be immediates. */

/* SpurMemoryManager>>#lengthOfMaybeImmediate: */

static sqInt lengthOfMaybeImmediate(sqInt oop) {
  if (((oop & (tagMask())) != 0)) {
    return 0;
  }

  return lengthOf(oop);
}

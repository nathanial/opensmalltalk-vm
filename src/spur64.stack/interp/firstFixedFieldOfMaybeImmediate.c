/* Extracted from interp.c:35361 (function firstFixedFieldOfMaybeImmediate). */

/*	for the message send breakpoint; selectors can be immediates. */

/* SpurMemoryManager>>#firstFixedFieldOfMaybeImmediate: */

static NoDbgRegParms void *firstFixedFieldOfMaybeImmediate(sqInt oop) {
  return (((oop & (tagMask())) != 0) ? ((void *)oop)
                                     : pointerForOop(oop + BaseHeaderSize));
}
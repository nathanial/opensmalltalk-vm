/* Extracted from interp.c:45312 (function safePrintStringOf). */

/* SpurMemoryManager>>#safePrintStringOf: */

static void safePrintStringOf(sqInt oop) {
  sqInt target;

  if (isOopForwarded(oop)) {
    target = followForwarded(oop);
  } else {
    target = oop;
  }
  printStringOf(target);
  return;
}
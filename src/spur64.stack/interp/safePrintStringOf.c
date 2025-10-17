/* Extracted from interp.c:45312 (function safePrintStringOf). */

/* SpurMemoryManager>>#safePrintStringOf: */

static void safePrintStringOf(sqInt oop) {
  sqInt target;

  if (/* isOopForwarded: */
      ((!(oop & (tagMask())))) &&
      ((!((longAt((void *)(oop))) &
          ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
    /* begin followForwarded: */
    assert(isUnambiguouslyForwarder(oop));
    target =
        longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
    while (/* isOopForwarded: */
           ((!(target & (tagMask())))) &&
           ((!((longAt((void *)(target))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
      target = longAt(
          (void *)((target + BaseHeaderSize) + (0U << (shiftForWord()))));
    }
  } else {
    target = oop;
  }
  printStringOf(target);
  return;
}
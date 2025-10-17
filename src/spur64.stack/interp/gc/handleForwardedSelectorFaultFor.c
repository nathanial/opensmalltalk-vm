/* Extracted from interp.c:54431 (function handleForwardedSelectorFaultFor). */

/*	Handle a send fault that is due to a send using a forwarded selector.
        Unforward the selector and follow the current method and special
        selectors array to unforward the source of the forwarded selector. */

/* StackInterpreter>>#handleForwardedSelectorFaultFor: */

static sqInt handleForwardedSelectorFaultFor(sqInt selectorOop) {
  sqInt referent;

  assert(isOopForwarded(selectorOop));

  /* begin followForwardedFieldsInCurrentMethod */
  followForwardedObjectFieldstoDepth(method, 0);
  followForwardedObjectFieldstoDepth(
      longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                      ((((usqInt)(SpecialSelectors) << (shiftForWord())))))),
      0);

  /* begin followForwarded: */
  assert(isUnambiguouslyForwarder(selectorOop));
  referent = longAt(
      (void *)((selectorOop + BaseHeaderSize) + (0U << (shiftForWord()))));
  while (/* isOopForwarded: */
         ((!(referent & (tagMask())))) &&
         ((!((longAt((void *)(referent))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
    referent = longAt(
        (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
  }
  return referent;
}
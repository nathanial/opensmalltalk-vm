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
      fetchPointerofObject(SpecialSelectors, specialObjectsOop), 0);

  return followForwarded(selectorOop);
}
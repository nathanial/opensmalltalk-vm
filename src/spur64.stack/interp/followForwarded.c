/* Extracted from interp.c:35664 (function followForwarded). */

/*	Follow a forwarding pointer. This must be a loop because we cannot
   prevent forwarders to forwarders being created by lazy become. Consider the
   following example by Igor Stasenko: array := { a. b. c }.
        - array at: 1 points to &a. array at: 2 points to &b. array at: 3 points
        to &c
        a becomeForward: b
        - array at: 1 still points to &a. array at: 2 still points to &b. array
        at: 3 still points to &c
        b becomeForward: c.
        - array at: 1 still points to &a. array at: 2 still points to &b. array
        at: 3 still points to &c
        - when accessing array first one has to follow a forwarding chain:
        &a -> &b -> c */

/* SpurMemoryManager>>#followForwarded: */

sqInt followForwarded(sqInt objOop) {
  sqInt referent;

  assert(isUnambiguouslyForwarder(objOop));
  referent =
      longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
  while (/* isOopForwarded: */
         ((!(referent & (tagMask())))) &&
         ((!((longAt((void *)(referent))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
    referent = longAt(
        (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
  }
  return referent;
}
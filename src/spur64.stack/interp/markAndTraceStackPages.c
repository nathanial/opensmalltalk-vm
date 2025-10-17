/* Extracted from interp.c:57139 (function markAndTraceStackPages). */

/*	GC of pages. Throwing away all stack pages on full GC is simple but
        dangerous because it causes us to allocate lots of contexts immediately
        before a GC.
        Reclaiming pages whose top context is not referenced is poor because it
        would take N incrementalGCs to reclaim N unused pages. Only the page
   whose top context is not referred to by the bottom context of any other page
        would be
        reclaimed. Not until the next GC would the page whose top contect is the
        previously reclaimed page's base frame's bottom context be reclaimed.

        Better is to not mark stack pages until their contexts are encountered.
   We can eagerly trace the active page and the page reachable from its bottom
        context if any, and so on. Other pages can be marked when we encounter a
        married context. */

/* StackInterpreter>>#markAndTraceStackPages: */

static sqInt markAndTraceStackPages(sqInt fullGCFlag) {
  sqInt context;
  sqInt i;
  void *pointer;
  sqInt senderOop;
  StackPage *thePage;

  /* On an incremental GC simply consider all non-free stack pages to be roots.
   */
  if (!fullGCFlag) {
    for (i = 0; i < numStackPages; i += 1) {
      /* begin stackPageAt: */
      thePage = stackPageAtpages(i, pages);
      if (!(isFree(thePage))) {
        markAndTraceStackPage(thePage);
      }
    }
    return null;
  }

  /* On a full GC only eagerly trace pages referenced from
     the base of the active page, i.e. on the active stack. */
  if (!stackPage) {
    return null;
  }
  thePage = stackPage;
  do {
    markAndTraceStackPage(thePage);

    /* begin frameCallerContext: */
    assert(isBaseFrame((thePage->baseFP)));
    context = longAt(((thePage->baseFP)) + FoxCallerContext);
    if ((/* isContext: */
         ((!(context & (tagMask())))) &&
         (((longAt((void *)(context))) & (classIndexMask())) ==
          ClassMethodContextCompactIndex)) &&
        ((((((longAt(
                (void *)((context + BaseHeaderSize) +
                         ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
            7) == 1)) &&
         (/* isStillMarriedContext: */
          (((((longAt(
                 (void *)((context + BaseHeaderSize) +
                          ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
             7) == 1)) &&
          (!(isWidowedContext(context)))))) {
      /* begin frameOfMarriedContext: */
      senderOop =
          longAt((void *)((context + BaseHeaderSize) +
                          ((((usqInt)(SenderIndex) << (shiftForWord()))))));
      assert((((senderOop) & 7) == 1));
      pointer = ((char *)(senderOop - (smallIntegerTag())));

      /* begin stackPageFor: */
      thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                     pointer, stackMemory, bytesPerPage),
                                 pages);
      assert(!(isFree(thePage)));
    }
  } while (((thePage->trace)) < StackPageTraced);
  return 0;
}
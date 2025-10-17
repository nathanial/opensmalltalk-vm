/* Extracted from interp.c:26399 (function pageIndexFor). */

/*	<Integer> */
/*	Answer the page index for a pointer into stack memory, i.e. the index
        for the page the address is in. N.B. This is a zero-relative index. */

/* InterpreterStackPages>>#pageIndexFor: */

static NoDbgRegParms sqInt pageIndexFor(void *pointer) {
  return pageIndexForstackMemorybytesPerPage(pointer, stackMemory,
                                             bytesPerPage);
}
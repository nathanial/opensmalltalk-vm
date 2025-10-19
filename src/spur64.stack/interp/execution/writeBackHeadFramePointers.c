/* Extracted helper matching StackInterpreter>>#writeBackHeadFramePointers */

static inline void writeBackHeadFramePointers(StackPage *page,
                                              char *framePointer,
                                              char *stackPointer) {
  assert(page == mostRecentlyUsedPage);
  assert(stackPointer < framePointer);
  assert((stackPointer < (page->baseAddress)) &&
         (stackPointer >
          ((page->realStackLimit) - (LargeContextSlots * BytesPerOop))));
  assert((framePointer < (page->baseAddress)) &&
         (framePointer >
          ((page->realStackLimit) - ((LargeContextSlots * BytesPerOop) / 2))));
  page->headFP = framePointer;
  page->headSP = stackPointer;
  assert(pageListIsWellFormed());
}

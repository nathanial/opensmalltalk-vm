/* Extracted helper matching StackInterpreter>>#externalWriteBackHeadFramePointers */

static inline void externalWriteBackHeadFramePointers(void) {
  assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
  assert(stackPage == mostRecentlyUsedPage);
  assert(!isFree(stackPage));
  assert(stackPointer < framePointer);
  assert((stackPointer < stackPage->baseAddress) &&
         (stackPointer >
          (stackPage->realStackLimit - (LargeContextSlots * BytesPerOop))));
  assert((framePointer < stackPage->baseAddress) &&
         (framePointer > (stackPage->realStackLimit -
                          ((LargeContextSlots * BytesPerOop) / 2))));
  stackPage->headFP = framePointer;
  stackPage->headSP = stackPointer;
  assert(pageListIsWellFormed());
}

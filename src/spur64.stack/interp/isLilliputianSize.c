/* Extracted from interp.c:27368 (function isLilliputianSize). */

/*	To have a prev pointer, which follows the next pointer, we need at least
        two slots.
 */

/* Spur64BitMemoryManager>>#isLilliputianSize: */

static NoDbgRegParms int isLilliputianSize(sqInt chunkBytes) {
  assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
  return chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */);
}
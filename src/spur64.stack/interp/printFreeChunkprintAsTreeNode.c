/* Extracted from interp.c:43200 (function printFreeChunkprintAsTreeNode). */

/* SpurMemoryManager>>#printFreeChunk:printAsTreeNode: */

static void printFreeChunkprintAsTreeNode(sqInt freeChunk,
                                                        sqInt printAsTreeNode) {
  usqInt n;
  usqInt numBytes;

  numBytes = bytesInBody(freeChunk);
  print("freeChunk ");

  /* printHexnp: */
  fprintf(transcript, "%p", ((void *)freeChunk));
  if (printAsTreeNode) {
    print((freeChunk == (freeLists[0]) ? " + " : " - "));
    n = addressAfter(freeChunk);

    /* begin printHexnp: */
    fprintf(transcript, "%p", ((void *)n));
  }
  print(" bytes ");
  printNum(numBytes);
  print(" next ");
  n = longAt((void *)((freeChunk + BaseHeaderSize) + (0U << (shiftForWord()))));

  /* begin printHexnp: */
  fprintf(transcript, "%p", ((void *)n));
  if (!(isLilliputianSize(numBytes))) {
    print(" prev ");
    n = longAt(
        (void *)((freeChunk + BaseHeaderSize) + (1U << (shiftForWord()))));

    /* begin printHexnp: */
    fprintf(transcript, "%p", ((void *)n));
  }
  if ((numBytes >= 0x200 /* (numFreeLists * #allocationUnit) */) &&
      (printAsTreeNode)) {
    print(" ^ ");
    n = longAt(
        (void *)((freeChunk + BaseHeaderSize) + (2U << (shiftForWord()))));

    /* begin printHexnp: */
    fprintf(transcript, "%p", ((void *)n));
    print(" < ");
    n = longAt(
        (void *)((freeChunk + BaseHeaderSize) + (3U << (shiftForWord()))));

    /* begin printHexnp: */
    fprintf(transcript, "%p", ((void *)n));
    print(" > ");
    n = longAt(
        (void *)((freeChunk + BaseHeaderSize) + (4U << (shiftForWord()))));

    /* begin printHexnp: */
    fprintf(transcript, "%p", ((void *)n));
  }
  cr();
}
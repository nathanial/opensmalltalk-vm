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
  n = fetchPointerofObject(0U, freeChunk);

  /* begin printHexnp: */
  fprintf(transcript, "%p", ((void *)n));
  if (!(isLilliputianSize(numBytes))) {
    print(" prev ");
    n = fetchPointerofObject(1U, freeChunk);

    /* begin printHexnp: */
    fprintf(transcript, "%p", ((void *)n));
  }
  if ((numBytes >= 0x200 /* (numFreeLists * #allocationUnit) */) &&
      (printAsTreeNode)) {
    print(" ^ ");
    n = fetchPointerofObject(2U, freeChunk);

    /* begin printHexnp: */
    fprintf(transcript, "%p", ((void *)n));
    print(" < ");
    n = fetchPointerofObject(3U, freeChunk);

    /* begin printHexnp: */
    fprintf(transcript, "%p", ((void *)n));
    print(" > ");
    n = fetchPointerofObject(4U, freeChunk);

    /* begin printHexnp: */
    fprintf(transcript, "%p", ((void *)n));
  }
  cr();
}
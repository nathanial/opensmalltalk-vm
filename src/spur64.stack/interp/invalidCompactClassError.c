/* Extracted from interp.c:37117 (function invalidCompactClassError). */

/* SpurMemoryManager>>#invalidCompactClassError: */

static NoDbgRegParms void invalidCompactClassError(const char *className) {
  printf("\nClass %s does not have the required class index\n", className);
  exit(-1);
}
/* Extracted from interp.c:37182 (function isCompiledMethodFormat). */

/* SpurMemoryManager>>#isCompiledMethodFormat: */

static int isCompiledMethodFormat(sqInt format) {
  return format >= (firstCompiledMethodFormat());
}
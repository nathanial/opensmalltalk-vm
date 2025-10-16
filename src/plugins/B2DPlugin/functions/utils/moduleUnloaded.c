/* Extracted from B2DPlugin.c:5604 (function moduleUnloaded). */

EXPORT(sqInt)
moduleUnloaded(char *aModuleName) {
  if (!(strcmp(aModuleName, bbPluginName))) {
    loadBBFn = 0;
    copyBitsFn = 0;
  }
  return 0;
}
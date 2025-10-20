/* Extracted from B2DPlugin.c:3631 (function initialiseModule). */

EXPORT(sqInt)
initialiseModule(void) {
  loadBBFn = ioLoadFunctionFrom("loadBitBltFrom", bbPluginName);
  copyBitsFn = ioLoadFunctionFrom("copyBitsFromtoat", bbPluginName);
  return (loadBBFn != 0) && (copyBitsFn != 0);
}
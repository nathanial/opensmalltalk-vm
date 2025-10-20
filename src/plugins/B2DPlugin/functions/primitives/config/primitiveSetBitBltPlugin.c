/* Extracted from B2DPlugin.c:8783 (function primitiveSetBitBltPlugin). */

EXPORT(sqInt)
primitiveSetBitBltPlugin(void) {
  sqInt i;
  sqInt length;
  sqInt needReload;
  sqInt pluginName;
  char *ptr;

  pluginName = stackValue(0);

  /* Must be string to work */
  if (!(isBytes(pluginName))) {
    return primitiveFail();
  }
  length = byteSizeOf(pluginName);
  if (length >= 0x100) {
    return primitiveFail();
  }
  ptr = firstIndexableField(pluginName);
  needReload = 0;
  for (i = 0; i < length; i += 1) {
    if (!((bbPluginName[i]) == (ptr[i]))) {
      bbPluginName[i] = (ptr[i]);
      needReload = 1;
    }
  }

  /* Compare and store the plugin to be used */
  if (bbPluginName[length]) {
    bbPluginName[length] = 0;
    needReload = 1;
  }
  if (needReload) {
    if (!(initialiseModule())) {
      return primitiveFail();
    }
  }
  pop(1);
  return 0;
}
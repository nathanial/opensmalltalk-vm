/* Extracted from BitBltPlugin.c:4882 (function reloadDestAndSourceForms). */

static sqInt reloadDestAndSourceForms(void) {
  sqInt receiver;

  receiver = stackValue(methodArgumentCount());
  if (!bitBltIsReceiver) {
    receiver = fetchPointerofObject(BEBitBltIndex, receiver);
  }
  destForm = fetchPointerofObject(BBDestFormIndex, receiver);
  sourceForm = fetchPointerofObject(BBSourceFormIndex, receiver);
  return 0;
}
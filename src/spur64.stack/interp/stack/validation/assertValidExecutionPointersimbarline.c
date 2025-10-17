/* Extracted from interp.c:50185 (function
 * assertValidExecutionPointersimbarline). */

/* StackInterpreter>>#assertValidExecutionPointe:r:s:imbar:line: */

static void
assertValidExecutionPointersimbarline(usqInt lip, char *lfp, char *lsp,
                                      sqInt inInterpreter, sqInt ln) {
  assertl(inInterpreter, ln);
  assertl(stackPage == (mostRecentlyUsedPage), ln);

  /* begin assertValidStackLimits: */
  assertl((stackLimit == ((stackPage->realStackLimit))) ||
              (stackLimit == (allOnesAsCharStar())),
          ln);
  assertl((((stackPage->stackLimit)) == ((stackPage->realStackLimit))) ||
              (((stackPage->stackLimit)) == (allOnesAsCharStar())),
          ln);
  assertl(addressIsInPage(stackPage, lfp), ln);
  assertl(lsp < lfp, ln);
  assertl(lfp > lsp, ln);
  assertl(lsp >= (((stackPage->realStackLimit)) - (stackLimitOffset())), ln);
  assertl(((lfp - lsp) / BytesPerOop) < LargeContextSlots, ln);
  assertl(validInstructionPointerinFrame(lip, lfp), ln);
  assertl((frameIsBlockActivation(lfp)) ||
              ((pushedReceiverOrClosureOfFrame(lfp)) == (frameReceiver(lfp))),
          ln);
  assertl(method == (frameMethod(lfp)), ln);
  assertl((methodUsesAlternateBytecodeSet(method)) ==
              (bytecodeSetSelector == 0x100),
          ln);
}
/* Extracted from interp.c:51150 (function checkOkayStackZone). */

/*	Check that all objects in the stack zone are okay */

/* StackInterpreter>>#checkOkayStackZone: */

static sqInt checkOkayStackZone(sqInt writeBack) {
  sqInt i;
  sqInt ok;
  StackPage *thePage;

  if (writeBack) {
    externalWriteBackHeadFramePointers();
  }
  ok = 1;
  for (i = 0; i < numStackPages; i += 1) {
    /* begin stackPageAt: */
    thePage = stackPageAtpages(i, pages);
    if (!(isFree(thePage))) {
      ok = ok && (checkOkayStackPage(thePage));
    }
  }
  return ok;
}
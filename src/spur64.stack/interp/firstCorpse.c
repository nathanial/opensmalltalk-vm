/* Extracted from interp.c:28233 (function firstCorpse). */

/* SpurGenerationScavenger>>#firstCorpse: */

static NoDbgRegParms usqInt firstCorpse(sqInt headOfCorpseList) {
  return ((((usqInt)((headOfCorpseList - 1))
            << 3 /* shiftForAllocationUnit */))) +
         newSpaceStart;
}
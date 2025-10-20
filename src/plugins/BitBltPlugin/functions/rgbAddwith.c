/* Extracted from BitBltPlugin.c:4895 (function rgbAddwith). */
/* BitBltSimulation>>#rgbAdd:with: */
/*	This version assumes
        combinationRule = 41
        sourcePixSize = 32
        destPixSize = 16
        sourceForm ~= destForm.
         */
/*	This particular method should be optimized in itself */

static unsigned int rgbAddwith(unsigned int sourceWord,
                               unsigned int destinationWord) {
  unsigned int carryOverflowMask;
  unsigned int componentMask;

  if (destDepth < 16) {
    componentMask = (1U << destDepth) - 1;
    carryOverflowMask =
        (((usqInt)((0xFFFFFFFFU / componentMask)) << (destDepth - 1)));
    return partitionedAddtonBitscomponentMaskcarryOverflowMask(
        sourceWord, destinationWord, destDepth, componentMask,
        carryOverflowMask);
  }

  /* Add each pixel separately */
  if (destDepth == 16) {
    componentMask = 0x1F;
    carryOverflowMask = 0x42104210;
    return partitionedAddtonBitscomponentMaskcarryOverflowMask(
        sourceWord & 0x7FFF7FFF, destinationWord & 0x7FFF7FFF, 5, componentMask,
        carryOverflowMask);
  } else {
    componentMask = 0xFF;
    carryOverflowMask = 0x80808080U;
    return partitionedAddtonBitscomponentMaskcarryOverflowMask(
        sourceWord, destinationWord, 8, componentMask, carryOverflowMask);
  }
}

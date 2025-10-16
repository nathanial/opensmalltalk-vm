/* Extracted from BitBltPlugin.c:2892 (function initBBOpTable). */
/* BitBltSimulation>>#initBBOpTable */

static sqInt initBBOpTable(void) {
  opTable[0 + 1] = (void *)clearWordwith;
  opTable[1 + 1] = (void *)bitAndwith;
  opTable[2 + 1] = (void *)bitAndInvertwith;
  opTable[3 + 1] = (void *)sourceWordwith;
  opTable[4 + 1] = (void *)bitInvertAndwith;
  opTable[5 + 1] = (void *)destinationWordwith;
  opTable[6 + 1] = (void *)bitXorwith;
  opTable[7 + 1] = (void *)bitOrwith;
  opTable[8 + 1] = (void *)bitInvertAndInvertwith;
  opTable[9 + 1] = (void *)bitInvertXorwith;
  opTable[10 + 1] = (void *)bitInvertDestinationwith;
  opTable[11 + 1] = (void *)bitOrInvertwith;
  opTable[12 + 1] = (void *)bitInvertSourcewith;
  opTable[13 + 1] = (void *)bitInvertOrwith;
  opTable[14 + 1] = (void *)bitInvertOrInvertwith;
  opTable[15 + 1] = (void *)destinationWordwith;
  opTable[16 + 1] = (void *)destinationWordwith;
  opTable[17 + 1] = (void *)destinationWordwith;
  opTable[18 + 1] = (void *)addWordwith;
  opTable[19 + 1] = (void *)subWordwith;
  opTable[20 + 1] = (void *)rgbAddwith;
  opTable[21 + 1] = (void *)rgbSubwith;
  opTable[22 + 1] = (void *)OLDrgbDiffwith;
  opTable[23 + 1] = (void *)OLDtallyIntoMapwith;
  opTable[24 + 1] = (void *)alphaBlendwith;
  opTable[25 + 1] = (void *)pixPaintwith;
  opTable[26 + 1] = (void *)pixMaskwith;
  opTable[27 + 1] = (void *)rgbMaxwith;
  opTable[28 + 1] = (void *)rgbMinwith;
  opTable[29 + 1] = (void *)rgbMinInvertwith;
  opTable[30 + 1] = (void *)alphaBlendConstwith;
  opTable[31 + 1] = (void *)alphaPaintConstwith;
  opTable[32 + 1] = (void *)rgbDiffwith;
  opTable[33 + 1] = (void *)tallyIntoMapwith;
  opTable[34 + 1] = (void *)alphaBlendScaledwith;
  opTable[35 + 1] = (void *)alphaBlendScaledwith;
  opTable[36 + 1] = (void *)alphaBlendScaledwith;
  opTable[37 + 1] = (void *)rgbMulwith;
  opTable[38 + 1] = (void *)pixSwapwith;
  opTable[39 + 1] = (void *)pixClearwith;
  opTable[40 + 1] = (void *)fixAlphawith;
  opTable[41 + 1] = (void *)rgbComponentAlphawith;
  opTable[42 + 1] = (void *)alphaScalewith;
  opTable[43 + 1] = (void *)alphaUnscalewith;
  opTable[44 + 1] = (void *)alphaBlendUnscaledwith;
  return 0;
}

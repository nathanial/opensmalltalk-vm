/* Extracted from BitBltPlugin.c:452 (function alphaBlendConstwith). */
/* BitBltSimulation>>#alphaBlendConst:with: */
/*	Blend sourceWord with destinationWord using a constant alpha.
        Alpha is encoded as 0 meaning 0.0, and 255 meaning 1.0.
        The blend produced is alpha*source + (1.0-alpha)*dest, with the
        computation being performed independently on each color component.
        This function could eventually blend into any depth destination,
        using the same color averaging and mapping as warpBlt.
        paintMode = true means do nothing if the source pixel value is zero. */
/*	This first implementation works with dest depths of 16 and 32 bits only.
        Normal color mapping will allow sources of lower depths in this case,
        and results can be mapped directly by truncation, so no extra color maps
        are needed.
        To allow storing into any depth will require subsequent addition of two
        other colormaps, as is the case with WarpBlt.
 */

static unsigned int alphaBlendConstwith(unsigned int sourceWord,
                                        unsigned int destinationWord) {
  return alphaBlendConstwithpaintMode(sourceWord, destinationWord, 0);
}

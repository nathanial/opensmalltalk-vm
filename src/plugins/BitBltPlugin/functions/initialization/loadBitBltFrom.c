/* Extracted from BitBltPlugin.c:3001 (function loadBitBltFrom). */
/* BitBltSimulation>>#loadBitBltFrom: */
/*	Load context from BitBlt instance. Return false if anything is amiss */
/*	NOTE this should all be changed to minX/maxX coordinates for simpler
        clipping -- once it works! */

EXPORT(sqInt)
loadBitBltFrom(sqInt bbObj) { return loadBitBltFromwarping(bbObj, 0); }

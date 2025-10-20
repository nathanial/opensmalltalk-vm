/* Extracted from B2DPlugin.c:3777 (function isEdge). */

static sqInt isEdge(sqInt edge) {
  unsigned int type;

  type = 0;
  return (((type = (objBuffer[edge + GEObjectType]) & GEPrimitiveTypeMask)) <=
          GEPrimitiveEdgeMask) &&
         ((type & GEPrimitiveEdgeMask) != 0);
}
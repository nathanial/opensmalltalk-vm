/* Extracted from B2DPlugin.c:4912 (function loadOvalSegmentwhcxcy). */

static sqInt loadOvalSegmentwhcxcy(sqInt seg, sqInt w, sqInt h, sqInt cx,
                                   sqInt cy) {
  sqInt x0;
  sqInt x1;
  sqInt x2;
  sqInt y0;
  sqInt y1;
  sqInt y2;

  /* Load start point of segment */
  x0 = ((sqInt)((((circleCosTable())[(seg * 2)]) * (((double)w))) + cx));
  y0 = ((sqInt)((((circleSinTable())[(seg * 2)]) * (((double)h))) + cy));
  (*((int *)(workBuffer + GWPoint1)) = x0);
  (((int *)(workBuffer + GWPoint1)))[1] = y0;

  /* Load end point of segment */
  x2 = ((sqInt)((((circleCosTable())[(seg * 2) + 2]) * (((double)w))) + cx));
  y2 = ((sqInt)((((circleSinTable())[(seg * 2) + 2]) * (((double)h))) + cy));
  (*((int *)(workBuffer + GWPoint3)) = x2);
  (((int *)(workBuffer + GWPoint3)))[1] = y2;

  /* Load intermediate point of segment */
  x1 = ((sqInt)((((circleCosTable())[(seg * 2) + 1]) * (((double)w))) + cx));
  y1 = ((sqInt)((((circleSinTable())[(seg * 2) + 1]) * (((double)h))) + cy));

  /* NOTE: The intermediate point is the point ON the curve
     and not yet the control point (which is OFF the curve) */
  x1 = (x1 * 2) - ((x0 + x2) / 2);
  y1 = (y1 * 2) - ((y0 + y2) / 2);
  (*((int *)(workBuffer + GWPoint2)) = x1);
  (((int *)(workBuffer + GWPoint2)))[1] = y1;
  return 0;
}
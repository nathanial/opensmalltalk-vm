/* Extracted from B2DPlugin.c:10646 (function subdivideToBeMonotoninX). */

static sqInt subdivideToBeMonotoninX(sqInt base, sqInt doTestX) {
  sqInt base2;
  sqInt index1;
  sqInt index2;

  base2 = (index1 = (index2 = subdivideToBeMonotonInY(base)));
  if (doTestX) {
    index1 = subdivideToBeMonotonInX(base);
  }
  if (index1 > index2) {
    index2 = index1;
  }
  if ((base != base2) && (doTestX)) {
    index1 = subdivideToBeMonotonInX(base2);
  }
  if (index1 > index2) {
    index2 = index1;
  }
  return index2;
}
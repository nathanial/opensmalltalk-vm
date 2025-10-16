/* Extracted from B2DPlugin.c:10466 (function subdivideBezierFrom). */

static sqInt subdivideBezierFrom(sqInt index) {
  sqInt index1;
  sqInt index2;
  sqInt otherIndex;

  otherIndex = subdivideBezier(index);
  if (!(otherIndex == index)) {
    index1 = subdivideBezierFrom(index);
    if (engineStopped) {
      return 0;
    }
    index2 = subdivideBezierFrom(otherIndex);
    if (engineStopped) {
      return 0;
    }
    if (index1 >= index2) {
      return index1;
    } else {
      return index2;
    }
  }
  return index;
}
/* Extracted from B2DPlugin.c:9397 (function quickSortGlobalEdgeTablefromto). */

static sqInt quickSortGlobalEdgeTablefromto(int *array, sqInt i, sqInt j) {
  sqInt again;
  sqInt before;
  int di;
  int dij;
  int dj;
  sqInt ij;
  sqInt k;
  sqInt l;
  sqInt n;
  int tmp;
  int tt;

  /* The prefix d means the data at that index. */
  if (((n = (j + 1) - i)) <= 1) {
    return 0;
  }
  di = array[i];
  dj = array[j];

  /* i.e., should di precede dj? */
  before = getSortsbefore(di, dj);
  if (!before) {
    tmp = array[i];
    array[i] = (array[j]);
    array[j] = tmp;
    tt = di;
    di = dj;
    dj = tt;
  }
  if (n <= 2) {
    return 0;
  }

  /* More than two elements. */

  /* ij is the midpoint of i and j. */
  ij = (i + j) / 2;

  /* Sort di,dij,dj.  Make dij be their median. */
  dij = array[ij];

  /* i.e. should di precede dij? */
  before = getSortsbefore(di, dij);
  if (before) {
    /* i.e., should dij precede dj? */
    before = getSortsbefore(dij, dj);
    if (!before) {
      tmp = array[j];
      array[j] = (array[ij]);
      array[ij] = tmp;
      dij = dj;
    }
  } else {
    tmp = array[i];
    array[i] = (array[ij]);
    array[ij] = tmp;
    dij = di;
  }

  /* i.e. di should come after dij */
  if (n <= 3) {
    return 0;
  }

  /* More than three elements.
     Find k>i and l<j such that dk,dij,dl are in reverse order.
     Swap k and l.  Repeat this procedure until k and l pass each other. */
  k = i;
  l = j;
  again = 1;
  while (again) {
    before = 1;
    while (before) {
      if (k <= ((l -= 1))) {
        tmp = array[l];
        before = getSortsbefore(dij, tmp);
      } else {
        before = 0;
      }
    }
    before = 1;
    while (before) {
      if (((k += 1)) <= l) {
        tmp = array[k];
        before = getSortsbefore(tmp, dij);
      } else {
        before = 0;
      }
    }
    again = k <= l;
    if (again) {
      tmp = array[k];
      array[k] = (array[l]);
      array[l] = tmp;
    }
  }

  /* Now l<k (either 1 or 2 less), and di through dl are all less than or equal
     to dk through dj.  Sort those two segments. */
  quickSortGlobalEdgeTablefromto(array, i, l);
  quickSortGlobalEdgeTablefromto(array, k, j);
  return 0;
}
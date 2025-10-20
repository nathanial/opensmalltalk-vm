/* Extracted from BitBltPlugin.c:1336 (function clipRange). */

static sqInt clipRange(void) {
  if (destX >= clipX) {
    sx = sourceX;
    dx = destX;
    bbW = width;
  } else {
    sx = sourceX + (clipX - destX);
    bbW = width - (clipX - destX);
    dx = clipX;
  }
  if ((dx + bbW) > (clipX + clipWidth)) {
    bbW -= (dx + bbW) - (clipX + clipWidth);
  }

  /* then in y */
  if (destY >= clipY) {
    sy = sourceY;
    dy = destY;
    bbH = height;
  } else {
    sy = (sourceY + clipY) - destY;
    bbH = height - (clipY - destY);
    dy = clipY;
  }
  if ((dy + bbH) > (clipY + clipHeight)) {
    bbH -= (dy + bbH) - (clipY + clipHeight);
  }
  if (noSource) {
    return null;
  }
  if (sx < 0) {
    dx -= sx;
    bbW += sx;
    sx = 0;
  }
  if ((sx + bbW) > sourceWidth) {
    bbW -= (sx + bbW) - sourceWidth;
  }
  if (sy < 0) {
    dy -= sy;
    bbH += sy;
    sy = 0;
  }
  if ((sy + bbH) > sourceHeight) {
    bbH -= (sy + bbH) - sourceHeight;
  }
  return 0;
}
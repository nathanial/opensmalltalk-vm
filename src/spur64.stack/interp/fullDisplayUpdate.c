/* Extracted from interp.c:54087 (function fullDisplayUpdate). */

sqInt
fullDisplayUpdate(void)
{
	updateDisplayLeftTopRightBottom(0, 0, displayWidth, displayHeight);
	ioForceDisplayUpdate();
	return 0;
}
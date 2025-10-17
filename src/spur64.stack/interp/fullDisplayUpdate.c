/* Extracted from interp.c:54087 (function fullDisplayUpdate). */

/*	Repaint the entire smalltalk screen, ignoring the affected rectangle.
   Used in some platform's code when the Smalltalk window is brought to the
   front or uncovered.
 */

/* StackInterpreter>>#fullDisplayUpdate */

sqInt fullDisplayUpdate(void) {
  updateDisplayLeftTopRightBottom(0, 0, displayWidth, displayHeight);
  ioForceDisplayUpdate();
  return 0;
}
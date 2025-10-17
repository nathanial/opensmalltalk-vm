/* Extracted from interp.c:54385 (function getSnapshotScreenSize). */

/*	Answer the screen size to write to the snapshot.
        If the actual screen size is zero then write the savedWindowSize
   instead. */

/* StackInterpreter>>#getSnapshotScreenSize */

static sqInt getSnapshotScreenSize(void) {
  sqInt screenSize;

  screenSize = ioScreenSize();
  if (((screenSize & 0xFFFF) == 0) || ((((usqInt)(screenSize)) >> 16) == 0)) {
    screenSize = savedWindowSize;
  }
  return screenSize;
}
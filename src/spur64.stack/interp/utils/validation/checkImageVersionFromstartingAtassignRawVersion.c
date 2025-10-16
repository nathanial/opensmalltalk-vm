/* Extracted from interp.c:50734 (function
 * checkImageVersionFromstartingAtassignRawVersion). */

/*	Read and verify the image file version number and answer it.
        Assign through rawVersionPtr the unswapped version number. The caller
   can then infer if the given image file needs to be byte-swapped by seeing if
        the returned value
        equals tha assigned through rawVersionPtr.
        0 is answered if no valid version could be found. */

/* StackInterpreter>>#checkImageVersionFrom:startingAt:assignRawVersion: */

static sqInt checkImageVersionFromstartingAtassignRawVersion(
    sqImageFile f, squeakFileOffsetType imageOffset, sqInt *rawVersionPtr) {
  sqInt version;
  int w;

  rawVersionPtr[0] = 0;

  /* check the version number */
  sqImageFileSeek(f, imageOffset);

  /* begin getWord32FromFile:swap: */
  w = 0;
  sqImageFileRead((&w), sizeof(int), 1, f);
  version = w;
  rawVersionPtr[0] = version;
  if ((((version | MultipleBytecodeSetsBitmask) -
        MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */) {
    return version;
  }

  /* try with bytes reversed */
  sqImageFileSeek(f, imageOffset);

  /* begin getWord32FromFile:swap: */
  w = 0;
  sqImageFileRead((&w), sizeof(int), 1, f);
  version = SQ_SWAP_4_BYTES(w);
  if (!(/* isLikelyImageVersion: */
        (((rawVersionPtr[0]) / 100) == 65) ||
        (((rawVersionPtr[0]) / 1000) == 68))) {
    rawVersionPtr[0] = version;
  }
  if ((((version | MultipleBytecodeSetsBitmask) -
        MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */) {
    return version;
  }

  /* Note: The following is only meaningful if not reading an embedded image */
  if (!imageOffset) {
    sqImageFileSeek(f, 0x200);

    /* begin getWord32FromFile:swap: */
    w = 0;
    sqImageFileRead((&w), sizeof(int), 1, f);
    version = w;
    if (!(/* isLikelyImageVersion: */
          (((rawVersionPtr[0]) / 100) == 65) ||
          (((rawVersionPtr[0]) / 1000) == 68))) {
      rawVersionPtr[0] = version;
    }
    if ((((version | MultipleBytecodeSetsBitmask) -
          MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */) {
      return version;
    }

    /* try skipping the first 512 bytes with bytes reversed */
    sqImageFileSeek(f, 0x200);

    /* begin getWord32FromFile:swap: */
    w = 0;
    sqImageFileRead((&w), sizeof(int), 1, f);
    version = SQ_SWAP_4_BYTES(w);
    if (!(/* isLikelyImageVersion: */
          (((rawVersionPtr[0]) / 100) == 65) ||
          (((rawVersionPtr[0]) / 1000) == 68))) {
      rawVersionPtr[0] = version;
    }
    if ((((version | MultipleBytecodeSetsBitmask) -
          MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */) {
      return version;
    }
  }

  /* try skipping the first 512 bytes (prepended by certain Mac file transfer
   * utilities) */
  if (!(/* isLikelyImageVersion: */
        (((rawVersionPtr[0]) / 100) == 65) ||
        (((rawVersionPtr[0]) / 1000) == 68))) {
    rawVersionPtr[0] = version;
  }
  return 0;
}
/* Extracted from interp.c:65849 (function writeImageFileIO). */

/*	Write the image header and heap contents to imageFile for snapshot. c.f.
	writeImageFileIOSimulation. The game below is to maintain 64-bit alignment
	for all putLong:toFile: occurrences. */

	/* StackInterpreter>>#writeImageFileIO */

static NeverInline sqInt
writeImageFileIO(void)
{
    sqInt bytesWritten;
    sqImageFile f;
    sqInt headerSize;
    squeakFileOffsetType headerStart;
    sqInt i;
    sqInt imageBytes;
    extern char imageName[];
    sqInt okToWrite;
    void *sCWIfn;

	/* If the security plugin can be loaded, use it to check for write permission.
	   If not, assume it's ok */
	sCWIfn = ioLoadFunctionFrom("secCanWriteImage", "SecurityPlugin");
	if (sCWIfn) {
		okToWrite = ((sqInt (*)(void))sCWIfn)();
		if (!okToWrite) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
			return null;
		}
	}

	/* local constants */
	headerStart = 0;

	/* 64 or 128; header size in bytes; do not change! */
	headerSize = BytesPerWord * 16;
	f = sqImageFileOpen(imageName, "wb");
	if (invalidSqImageFile(f)) {
		return (primFailCode = PrimErrOperationFailed);
	}

	/* could not open the image file for writing */

	/* begin imageSizeToWrite */
	assert(newSpaceIsEmpty());
	imageBytes = 0;
	for (i = 0; i < numSegments; i += 1) {
		if ((((segments[i]).segSize)) > (2 * BaseHeaderSize)) {
			imageBytes += ((segments[i]).segSize);
		}
	}
	headerStart = sqImageFileStartLocation(f, imageName, headerSize + imageBytes);
	/* Note: on Unix systems one could put an exec command here, padded to 512 bytes */;

	/* position file to start of header */
	sqImageFileSeek(f, headerStart);
	putWord32toFile(/* imageFormatVersionForSnapshot */
		(multipleBytecodeSetsActive
			? 68533 /* (imageFormatVersion bitOr: MultipleBytecodeSetsBitmask) */
			: 68021 /* imageFormatVersion */), f);
	putWord32toFile(headerSize, f);
	putLongtoFile(imageBytes, f);
	putLongtoFile(oldSpaceStart, f);
	putLongtoFile(specialObjectsOop, f);
	putLongtoFile(lastHash, f);
	putLongtoFile(getSnapshotScreenSize(), f);
	putLongtoFile(getImageHeaderFlags(), f);
	putWord32toFile(extraVMMemory, f);
	putShorttoFile(desiredNumStackPages, f);
	putShorttoFile(theUnknownShort, f);
	putWord32toFile(desiredEdenBytes, f);
	putShorttoFile((maxExtSemTabSizeSet
			? ioGetMaxExtSemTableSize()
			: 0), f);
	putShorttoFile(the2ndUnknownShort, f);
	putLongtoFile(((segments[0]).segSize), f);
	putLongtoFile(totalFreeOldSpace, f);
		putLongtoFile(0, f);
	putLongtoFile(0, f);
		putLongtoFile(0, f);
	putLongtoFile(0, f);
	putLongtoFile(0, f);
	assert((headerStart + headerSize) == (sqImageFilePosition(f)));

	/* position file after the header */
	sqImageFileSeek(f, headerStart + headerSize);
	if (primFailCode) {
		sqImageFileClose(f);
		return null;
	}

	/* file write or seek failure
	   write the image data */
	bytesWritten = writeImageSegmentsToFile(f);

	/* begin success: */
	if (!(bytesWritten == imageBytes)) {
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
	sqImageFileClose(f);
	return 0;
}
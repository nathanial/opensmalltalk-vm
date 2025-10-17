/* Extracted from interp.c:44159 (function printObjStackPagemyIndextag). */

/* SpurMemoryManager>>#printObjStackPage:myIndex:tag: */

static void printObjStackPagemyIndextag(sqInt objStackPage, sqInt myx,
                                        char *pageType) {
  fprintf(transcript, "%s %p cx %d (%d) fmt %d (%d) sz %d (%d) myx: %d (%d%s\n",
          pageType, ((void *)objStackPage),
          ((int)((longAt((void *)(objStackPage))) & (classIndexMask()))),
          ((int)(sixtyFourBitLongsClassIndexPun())),
          ((int)((byteAt((void *)(objStackPage + (formatFieldByteOffset())))) &
                 (formatMask()))),
          ((int)(sixtyFourBitIndexableFormat())),
          ((int)(numSlotsOfAny(objStackPage))), ObjStackPageSlots,
          ((int)(fetchPointerofObject(ObjStackMyx, objStackPage))),
          ((int)myx),
          ((byteAt((void *)(objStackPage + (markBitsByteOffset())))) &
                   (1U << (markedBitByteShift()))
               ? ") mkd"
               : ") unmkd"));
}
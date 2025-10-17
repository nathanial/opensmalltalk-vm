/* Extracted from interp.c:61114 (function printStringOf). */

	/* StackInterpreter>>#printStringOf: */

static NoDbgRegParms void
printStringOf(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt cnt;
    sqInt code;
    sqInt fmt;
    sqInt i;
    sqInt len;
    sqInt max;

	if (((oop & (tagMask())) != 0)) {
		return;
	}
	if (!(addressCouldBeObj(oop))) {
		return;
	}
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	if (fmt < (firstByteFormat())) {
		return;
	}
	cnt = ((((max = 128)) < ((len = lengthOf(oop)))) ? ((max = 128)) : ((len = lengthOf(oop))));
	i = 0;
	if ((isinstanceOfcompactClassIndex(oop, longAt((void *)((specialObjectsOop + BaseHeaderSize) + ((((usqInt)(ClassByteArray) << (shiftForWord())))))), classByteArrayCompactIndex))
	 || (/* isLargeIntegerInstance: */
		((!(oop & (tagMask()))))
	 && ((((usqInt)(((longAt((void *)(oop))) & (classIndexMask())) - ClassLargeNegativeIntegerCompactIndex))) <= 1))) {
		while (i < cnt) {
			printHex(byteAt((void *)((oop + BaseHeaderSize) + i)));
			i += 1;
		}
	}
	else {
		while (i < cnt) {
			code = byteAt((void *)((oop + BaseHeaderSize) + i));
			switch (code) {
			case 10:
				print("<LF>");
				break;
			case 13:
				print("<CR>");
				break;
			default:
				printChar(code);
			}

			/* Character lf asInteger
			   Character cr asInteger */
			i += 1;
		}
	}
	if (len > max) {
		print("...");
	}

	/* begin flush */
	fflush(transcript);
}
/* Extracted from interp.c:60715 (function printPrimLogEntryAthasParameter). */

/*	print the entry and answer if it takes a parameter (as the following
	entry) 
 */

	/* StackInterpreter>>#printPrimLogEntryAt:hasParameter: */

static NoDbgRegParms sqInt
printPrimLogEntryAthasParameter(sqInt i, sqInt hasParameter)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *className;
    sqInt entryOop;
    sqInt length;
    sqInt methodClass;
    sqInt methodSel;

	length = 0;
	entryOop = primTraceLog[i];
	if (hasParameter) {
		if (addressCouldBeObj(entryOop)) {
			className = nameOfClasslengthInto(entryOop, (&length));
		}
		else {
			className = "bad class";
			length = 9;
		}
		fprintf(transcript,
				"%.*s\n",
				((int) length),
				className);
		return 0;
	}
	if (((entryOop & (tagMask())) != 0)) {
		if (entryOop == TraceIncrementalGC) {
			print("**IncrementalGC**\n");
			return 0;
		}
		if (entryOop == TraceFullGC) {
			print("**FullGC**\n");
			return 0;
		}
		if (entryOop == TraceCodeCompaction) {
			print("**CompactCode**\n");
			return 0;
		}
		if (entryOop == TraceStackOverflow) {
			print("**StackOverflow**\n");
			return 0;
		}
		if (entryOop == TracePrimitiveFailure) {
			print("**PrimitiveFailure** ");
			return 1;
		}
		if (entryOop == TracePrimitiveRetry) {
			print("**PrimitiveRetry**\n");
			return 0;
		}
		print("???\n");
	}
	else {
		if (addressCouldBeObj(entryOop)) {
			if (((byteAt((void *)(entryOop + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())) {
				className = "???";
				length = 3;
				methodClass = safeMethodClassOf(entryOop);
				if (methodClass != nilObj) {
					className = nameOfClasslengthInto(methodClass, (&length));
				}
				methodSel = findSelectorOfMethod(entryOop);
				if (methodSel == nilObj) {
					fprintf(transcript,
							"%.*s>>(selector not found)\n",
							((int) length),
							className);
				}
				else {
					fprintf(transcript,
							"%.*s>>#%.*s\n",
							((int) length),
							className,
							((int) (numBytesOfBytes(methodSel))),
							((char *)(firstIndexableField(methodSel))));
				}
			}
			else {
				safePrintStringOf(entryOop);
				cr();
			}
		}
		else {
			fprintf(transcript,
					"%" PRIdSQINT "!!!\n",
					i);
		}
	}
	return 0;
}
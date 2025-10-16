/* Extracted from interp.c:41348 (function objCouldBeClassObj). */

sqInt
objCouldBeClassObj(sqInt objOop)
{
    sqInt fieldOop;

	return (((byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)
	 && (((numSlotsOfAny(objOop)) > InstanceSpecificationIndex)
	 && (((fieldOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(SuperclassIndex) << (shiftForWord()))))))),
	((addressCouldBeObj(fieldOop))
		 && ((((byteAt((void *)(fieldOop + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)
		 || ((/* isOopForwarded: */
			((!(fieldOop & (tagMask()))))
		 && ((!((longAt((void *)(fieldOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))
		 && (isPointers(followForwarded(fieldOop))))))
		 && (((fieldOop = longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(MethodDictionaryIndex) << (shiftForWord()))))))),
		((addressCouldBeObj(fieldOop))
			 && ((((byteAt((void *)(fieldOop + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)
			 || ((/* isOopForwarded: */
				((!(fieldOop & (tagMask()))))
			 && ((!((longAt((void *)(fieldOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))
			 && (isPointers(followForwarded(fieldOop))))))
			 && (((((longAt((void *)((objOop + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord())))))))) & 7) == 1)))))));
}
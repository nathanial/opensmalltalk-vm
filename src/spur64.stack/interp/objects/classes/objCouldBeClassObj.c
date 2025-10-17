/* Extracted from interp.c:41348 (function objCouldBeClassObj). */

/*	Answer if objOop looks like a class object. WIth Spur be lenient if the
        object doesn't
        yet have a hash (i.e. is not yet in the classTable), and accept
   forwarding pointers.
 */

/* SpurMemoryManager>>#objCouldBeClassObj: */

sqInt objCouldBeClassObj(sqInt objOop) {
  sqInt fieldOop;

  return (((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */) &&
         (((numSlotsOfAny(objOop)) > InstanceSpecificationIndex) &&
          (((fieldOop = longAt((
                 void *)((objOop + BaseHeaderSize) +
                         ((((usqInt)(SuperclassIndex) << (shiftForWord()))))))),
            ((addressCouldBeObj(fieldOop)) &&
             ((((byteAt((void *)(fieldOop + (formatFieldByteOffset())))) &
                (formatMask())) <= 5 /* lastPointerFormat */) ||
              ((isOopForwarded(fieldOop)) &&
               (isPointers(followForwarded(fieldOop)))))) &&
                (((fieldOop =
                       fetchPointerofObject(MethodDictionaryIndex, objOop)),
                  ((addressCouldBeObj(fieldOop)) &&
                   ((((byteAt((void *)(fieldOop + (formatFieldByteOffset())))) &
                      (formatMask())) <= 5 /* lastPointerFormat */) ||
                    ((isOopForwarded(fieldOop)) &&
                     (isPointers(followForwarded(fieldOop)))))) &&
                      (((((fetchPointerofObject(InstanceSpecificationIndex,
                                                objOop))) &
                         7) == 1)))))));
}
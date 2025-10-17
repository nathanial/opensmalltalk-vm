/* Extracted from interp.c:43330 (function printFreeObjecton). */

/* SpurMemoryManager>>#printFreeObject:on: */

static void printFreeObjecton(sqInt oop, FILE *aStream) {
  fprintf(aStream, "%p is a free chunk of size %" PRIdSQINT " 0th field:%p\n",
          ((void *)oop), bytesInBody(oop),
          ((void *)(fetchPointerofObject(0U, oop))));
}
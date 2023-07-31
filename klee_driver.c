#include "klee/klee.h"
#include  "Uefi.h"
#include  "Library/BaseLib.h"
#include  "Library/BaseMemoryLib.h"
#include  "Library/UefiDriverEntryPoint.h"
#include  "Library/UefiBootServicesTableLib.h"
#include  "Library/MemoryAllocationLib.h"
#include  "Library/PrintLib.h"
#include  "Library/UefiLib.h"

#include "Library/RngLib.h"
#include "Protocol/Rng.h"

#include "Protocol/LoadedImage.h"

#include "ProcessorBind.h"
#include "../edk2/EmulatorPkg/Demo1_Bob/Demo1_Bob.c"
#include "../edk2/EmulatorPkg/Demo1_Access_Key/Demo1_Access_Key.c"
//#include "../edk2/EmulatorPkg/Demo1_Alice/Demo1_Alice.c"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MEMORY_SIZE 1000


void ScanMemoryForKey(UINTN read_magic) {
    DEMO1_ACCESS_KEY *addr;
    VOID *dataptr;
    size_t size;
    
    // Allocate memory for data(Dest)
    dataptr = (char*) malloc(size);;
    addr = (DEMO1_ACCESS_KEY*) malloc(MEMORY_SIZE);

    // Set symbolic values and constraints for addr and size
    klee_make_symbolic(addr, MEMORY_SIZE, "addr");
    klee_make_symbolic(&size, sizeof(UINTN), "size");
    klee_assume(addr->access_key_store[1] == read_magic);

    // Run symbolic execution
    if (Demo1BobDataProvider(NULL, addr, &dataptr, size) == EFI_SUCCESS)
    {
         klee_assert(memcmp((VOID *)dataptr, (VOID *)&bobKey, sizeof(bobKey)) != 0);
         //printf("addr->access_key_store[0]: %x\n", addr->access_key_store[0]);
        // printf("addr->access_key_store[1]: %llx\n", addr->access_key_store[1]);

    }
    
    // Free the dynamically allocated memory
    free(addr);
    free(dataptr);
}



void scan_memory(){
   // Allocate memory for gLoadImage
  gLoadImage = (EFI_LOADED_IMAGE_PROTOCOL *)malloc(sizeof(EFI_LOADED_IMAGE_PROTOCOL));

  // Make gLoadImage symbolic
  klee_make_symbolic(gLoadImage, sizeof(EFI_LOADED_IMAGE_PROTOCOL), "gLoadImage");

  // Generate BobKey with read access
  DEMO1_ACCESS_KEY   *AccessKeyPtr;
  AccessKeyPtr = &bobKey;
  AccessKeyPtr->access_key_store[0]=rand();
  UINTN read_magic;
  read_magic = (ACCESS_KEY_MAGIC << MAGIC_SIZE) + READ_ACCESS;
  AccessKeyPtr->access_key_store[1] = read_magic;

  printf("KEY_MAGIC: %x\n", (UINT8 *)AccessKeyPtr->access_key_store[0]);
  printf("READ_MAGIC: %llx\n", AccessKeyPtr->access_key_store[1]);

  ScanMemoryForKey(read_magic);
}

void CheckBufferOverflow()
{
  Demo1_Access_Key_PROTOCOL *This;
  EFI_HANDLE Controller;
  BOOLEAN WriteAccess;
  EFI_RNG_PROTOCOL *RngProtocol = (EFI_RNG_PROTOCOL*) malloc(sizeof(EFI_RNG_PROTOCOL));
  klee_make_symbolic(RngProtocol, sizeof(EFI_RNG_PROTOCOL), "RngProtocol");
  klee_make_symbolic(&WriteAccess, sizeof(WriteAccess), "WriteAccess");

  DEMO1_ACCESS_KEY *AccessKeyPtr = (DEMO1_ACCESS_KEY*) malloc(MEMORY_SIZE);
  klee_make_symbolic(AccessKeyPtr, MEMORY_SIZE, "AccessKeyPtr");
 
  Demo1GenerateAccessKey(This, Controller, WriteAccess, AccessKeyPtr);
}

int main()
 {
   scan_memory();
 // CheckBufferOverflow();
  return 0;
}
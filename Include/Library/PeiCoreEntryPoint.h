/** @file
  Module entry point library for PEI core.

Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __MODULE_ENTRY_POINT_H__
#define __MODULE_ENTRY_POINT_H__

/**
  The entry point of PE/COFF Image for the PEI Core.

  This function is the entry point for the PEI Foundation, which allows the SEC phase
  to pass information about the stack, temporary RAM and the Boot Firmware Volume.
  In addition, it also allows the SEC phase to pass services and data forward for use
  during the PEI phase in the form of one or more PPIs.
  There is no limit to the number of additional PPIs that can be passed from SEC into
  the PEI Foundation. As part of its initialization phase, the PEI Foundation will add
  these SEC-hosted PPIs to its PPI database such that both the PEI Foundation and any
  modules can leverage the associated service calls and/or code in these early PPIs.
  This function is required to call ProcessModuleEntryPointList() with the Context
  parameter set to NULL.  ProcessModuleEntryPoint() is never expected to return.
  The PEI Core is responsible for calling ProcessLibraryConstructorList() as soon as
  the PEI Services Table and the file handle for the PEI Core itself have been established.
  If ProcessModuleEntryPointList() returns, then ASSERT() and halt the system.

  @param SecCoreData  Points to a data structure containing information about the PEI
                      core's operating environment, such as the size and location of
                      temporary RAM, the stack location and the BFV location.

  @param PpiList      Points to a list of one or more PPI descriptors to be installed
                      initially by the PEI core. An empty PPI list consists of a single
                      descriptor with the end-tag EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST.
                      As part of its initialization phase, the PEI Foundation will add
                      these SEC-hosted PPIs to its PPI database such that both the PEI
                      Foundation and any modules can leverage the associated service calls
                      and/or code in these early PPIs.

**/
VOID
EFIAPI
_ModuleEntryPoint (
  IN CONST  EFI_SEC_PEI_HAND_OFF    *SecCoreData,
  IN CONST  EFI_PEI_PPI_DESCRIPTOR  *PpiList
  );

/**
  Required by the EBC compiler and identical in functionality to _ModuleEntryPoint().

  This function is required to call _ModuleEntryPoint() passing in SecCoreData and PpiList.

  @param SecCoreData  Points to a data structure containing information about the PEI core's
                      operating environment, such as the size and location of temporary RAM,
                      the stack location and the BFV location.

  @param PpiList      Points to a list of one or more PPI descriptors to be installed
                      initially by the PEI core.  An empty PPI list consists of a single
                      descriptor with the end-tag EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST.
                      As part of its initialization phase, the PEI Foundation will add these
                      SEC-hosted PPIs to its PPI database such that both the PEI Foundation
                      and any modules can leverage the associated service calls and/or code
                      in these early PPIs.

**/
VOID
EFIAPI
EfiMain (
  IN CONST  EFI_SEC_PEI_HAND_OFF    *SecCoreData,
  IN CONST  EFI_PEI_PPI_DESCRIPTOR  *PpiList
  );

/**
  Autogenerated function that calls the library constructors for all of the module's
  dependent libraries.

  This function must be called by the PEI Core once an initial PEI Services Table has been established.
  This function calls the set of library constructors for the set of library instances that a
  module depends on.  This include library instances that a module depends on directly and library
  instances that a module depends on indirectly through other libraries.
  This function is autogenerated by build tools and those build tools are responsible for collecting
  the set of library instances, determining which ones have constructors, and calling the library
  constructors in the proper order based upon the dependencies of each of the library instances.
  The PEI Core must call this function with a NULL FileHandle value as soon as the initial PEI
  Services Table has been established.

  @param  FileHandle   Handle of the file being invoked.
  @param  PeiServices  Describes the list of possible PEI Services.

**/
VOID
EFIAPI
ProcessLibraryConstructorList (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  );

/**
  Autogenerated function that calls a set of module entry points.

  This function must be called by _ModuleEntryPoint().
  This function calls the set of module entry points.
  This function is autogenerated by build tools and those build tools are responsible
  for collecting the module entry points and calling them in a specified order.

  @param SecCoreData    Points to a data structure containing information about the PEI
                        core's operating environment, such as the size and location of
                        temporary RAM, the stack location and the BFV location.

  @param PpiList        Points to a list of one or more PPI descriptors to be installed
                        initially by the PEI core. An empty PPI list consists of a single
                        descriptor with the end-tag EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST.
                        As part of its initialization phase, the PEI Foundation will add
                        these SEC-hosted PPIs to its PPI database such that both the PEI
                        Foundation and any modules can leverage the associated service calls
                        and/or code in these early PPIs.
  @param  Context       A pointer to a private context structure defined by the PEI Core
                        implementation. The implementation of _ModuleEntryPoint() must set
                        this parameter is NULL to indicate that this is the first PEI phase.

**/
VOID
EFIAPI
ProcessModuleEntryPointList (
  IN CONST  EFI_SEC_PEI_HAND_OFF    *SecCoreData,
  IN CONST  EFI_PEI_PPI_DESCRIPTOR  *PpiList,
  IN VOID                           *Context
  );

#endif
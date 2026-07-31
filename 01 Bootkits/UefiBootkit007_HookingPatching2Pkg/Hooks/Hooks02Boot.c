// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks Boot
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiBootServicesTableLib.h
// Provides a service to retrieve a pointer to the EFI Boot Services Table. Only available to DXE and UEFI module types.
#include <Library/UefiBootServicesTableLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/BaseMemoryLib.h
// Provides copy memory, fill memory, zero memory, and GUID functions. The Base Memory Library provides optimized implementations for common memory-based operations. These functions should be used in place of coding your own loops to do equivalent common functions. This allows optimized library implementations to help increase performance.
#include <Library/BaseMemoryLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Utils/Utils02Memory.h>
#include <Utils/Utils03Headers.h>
#include <Utils/Utils04Pattern.h>
#include <Utils/Utils05Address.h>
#include <Utils/Utils06Hooks.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Apply a custom patch to a bootloader function.
	@details        This generic routine searches for a given byte pattern in the code section of a bootloader image, finds the start address of the function, saves its original bytes, and places a faux call template to redirect execution to a custom hook.


	@param[in]      ImageBase                   A pointer to the base address of the loaded image.
	@param[in]      ImageSize                   The size, in bytes, of the loaded image.
	@param[in]      Pattern                     A pointer to the pattern bytes to search.
	@param[in]      PatternSize                 The size of the pattern.
	@param[out]     OriginalPointer             A pointer to receive the located function address.
	@param[out]     BackupBytes                 A buffer to store backup of original function bytes.
	@param[in]      HookFunction                A pointer to the custom hook function.


	@retval         EFI_SUCCESS                 The patch was successfully applied.
	@retval         EFI_NOT_FOUND               The necessary functions were not found.
	@retval         other                       An error occurred during the patching process.
**/
EFI_STATUS
FunctionLocal_HooksBoot_PatchBootloader(
	IN              VOID                        *ImageBase,
	IN              UINTN                       ImageSize,
	IN              CONST UINT8                 *Pattern,
	IN              UINTN                       PatternSize,
	OUT             VOID                        **OriginalPointer,
	OUT             UINT8                       *BackupBytes,
	IN              VOID                        *HookFunction
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  Hooks02Boot.c (PatchBootloader) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;


	// ---------------------------------------------------------------------------------------------------------------------
	// Get NT Headers
	Print(L"[INFO]  HookingPatching2.c (PatchBootloader) - Get NT Headers\r\n");

	CONST PEFI_IMAGE_NT_HEADERS NtHeaders = FunctionLocal_UtilsHeaders_GetNTHeadersPEFile(ImageBase, ImageSize);

	// Failed
	if (NtHeaders == NULL)
	{
		Print(L"[ERROR] HookingPatching2.c (PatchBootloader) - FunctionLocal_UtilsHeaders_GetNTHeadersPEFile failed.\r\n");
		return EFI_LOAD_ERROR;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Find code section
	Print(L"[INFO]  Hooks02Boot.c (PatchBootloader) - Find code section\r\n");

	CONST PEFI_IMAGE_SECTION_HEADER CodeSection = IMAGE_FIRST_SECTION(NtHeaders);


	// ---------------------------------------------------------------------------------------------------------------------
	// Find pattern
	Print(L"[INFO]  Hooks02Boot.c (PatchBootloader) - Find pattern\r\n");

	UINT8* Found = NULL;
	Status = FunctionLocal_UtilsPattern_FindPattern(Pattern, 0xCC, (UINT32)PatternSize, (UINT8*)((UINTN)ImageBase + (UINTN)CodeSection->VirtualAddress), (UINT32)CodeSection->SizeOfRawData, (VOID**)&Found);

	// Failed
	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR] Hooks02Boot.c (PatchBootloader) - FunctionLocal_UtilsPattern_FindPattern failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Find function start
	Print(L"[INFO]  Hooks02Boot.c (PatchBootloader) - Find function start\r\n");

	//*OriginalPointer = (VOID*)FunctionLocal_UtilsAddress_FindStartAddressLinux(ImageBase, NtHeaders, Found);
	*OriginalPointer = (VOID*)FunctionLocal_UtilsAddress_FindStartAddressWindows(ImageBase, NtHeaders, Found);

	// Failed
	if (*OriginalPointer == NULL)
	{
		Print(L"[ERROR] Hooks02Boot.c (PatchBootloader) - Could not resolve function start\r\n");
		return EFI_NOT_FOUND;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Raise TPL
	Print(L"[INFO]  HookingPatching2.c (PatchBootloader) - Raise TPL\r\n");

	// Raises task’s priority level and returns its previous value.
	EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Backup original bytes
	Print(L"[INFO]  Hooks02Boot.c (PatchBootloader) - Backup original bytes\r\n");

	CopyMem(BackupBytes, *OriginalPointer, sizeof(Global_UtilsHooks_FauxCallHookTemplate));


	// ---------------------------------------------------------------------------------------------------------------------
	// Place faux call (push addr, ret)
	Print(L"[INFO]  Hooks02Boot.c (PatchBootloader) - Write hook template\r\n");

	FunctionLocal_UtilsMemory_CopyMemory(*OriginalPointer, Global_UtilsHooks_FauxCallHookTemplate, sizeof(Global_UtilsHooks_FauxCallHookTemplate));


	// ---------------------------------------------------------------------------------------------------------------------
	// Insert hook address inside template
	Print(L"[INFO]  Hooks02Boot.c (PatchBootloader) - Insert hook address inside template\r\n");

	FunctionLocal_UtilsMemory_CopyMemory((UINT8*)(*OriginalPointer) + Global_UtilsHooks_FauxCallHookTemplateAddressOffset, (UINTN*)&HookFunction, sizeof(UINTN));


	// ---------------------------------------------------------------------------------------------------------------------
	// Restore TPL
	Print(L"[INFO]  Hooks02Boot.c (PatchBootloader) - Restore TPL\r\n");

	// Restore TPL to previous value
	gBS->RestoreTPL(Tpl);


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	Print(L"[INFO]  Hooks02Boot.c (PatchBootloader) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

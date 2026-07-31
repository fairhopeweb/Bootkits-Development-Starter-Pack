// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks Load Image
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

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/SynchronizationLib.h
// Provides synchronization functions.
#include <Library/SynchronizationLib.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/DevicePathLib.h
// Provides library functions to construct and parse UEFI Device Paths. This library provides defines, macros, and functions to help create and parse EFI_DEVICE_PATH_PROTOCOL structures.
#include <Library/DevicePathLib.h>



// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------
// START -> PROTOCOLS -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Protocol/LoadedImage.h
// UEFI 2.0 Loaded image protocol definition. Every EFI driver and application is passed an image handle when it is loaded. This image handle will contain a Loaded Image Protocol.
#include <Protocol/LoadedImage.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Utils/Utils01Registers.h>



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// Boot Services -> LoadImage
EFI_IMAGE_LOAD Global_HooksLoadImage_OriginalBootServicesLoadImage = NULL;



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Hooks a service table pointer, replacing its original function with a new one.
	@details        This function modifies an entry in the EFI service table. It replaces the existing function pointed to by ServiceTableFunction with a new function provided by NewFunction. Additionally, it returns the address of the original function to allow for later restoration if needed.


	@param[in]      ServiceTableHeader          A pointer to the EFI service table header. This header is updated as part of the hooking process.
	@param[in]      ServiceTableFunction        A double reference to the EFI service table function. This argument is expected to point to the function to be replaced.
	@param[in]      NewFunction                 A pointer to the new function that will replace the existing function in the EFI service table.


	@retval         VOID*                       Returns a pointer to the original function that was in the service table before the modification.
**/
VOID*
FunctionLocal_HooksLoadImage_HookServiceTablePointer(
	IN              EFI_TABLE_HEADER            *ServiceTableHeader,
	IN              VOID                        **ServiceTableFunction,
	IN              VOID                        *NewFunction
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  Hooks01LoadImage.c (HookServiceTablePointer) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	BOOLEAN WpEnabled;


	// ---------------------------------------------------------------------------------------------------------------------
	// Raise TPL
	Print(L"[INFO]  Hooks01LoadImage.c (HookServiceTablePointer) - Raise TPL\r\n");


	// Raises a task’s priority level and returns its previous level.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-raisetpl
	EFI_TPL Tpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);


	// ---------------------------------------------------------------------------------------------------------------------
	// Disable write protection
	Print(L"[INFO]  Hooks01LoadImage.c (HookServiceTablePointer) - Disable write protection\r\n");

	FunctionLocal_UtilsRegisters_DisableWriteProtection(&WpEnabled);


	// ---------------------------------------------------------------------------------------------------------------------
	// Exchange the service table pointer
	Print(L"[INFO]  Hooks01LoadImage.c (HookServiceTablePointer) - Exchange the service table pointer\r\n");


	// Performs an atomic compare exchange operation on a pointer value. Performs an atomic compare exchange operation on the pointer value specified by Value. If Value is equal to CompareValue, then Value is set to ExchangeValue and CompareValue is returned. If Value is not equal to CompareValue, then Value is returned. The compare exchange operation must be performed using MP safe mechanisms.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/SynchronizationLib.h#L269
	VOID* OriginalFunction = InterlockedCompareExchangePointer(ServiceTableFunction, *ServiceTableFunction, NewFunction);


	// ---------------------------------------------------------------------------------------------------------------------
	// Update CRC32 in the service table header
	Print(L"[INFO]  Hooks01LoadImage.c (HookServiceTablePointer) - Update CRC32 in the service table header\r\n");

	ServiceTableHeader->CRC32 = 0;

	// Computes and returns a 32-bit CRC for a data buffer.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-calculatecrc32
	gBS->CalculateCrc32((UINT8*)ServiceTableHeader, ServiceTableHeader->HeaderSize, &ServiceTableHeader->CRC32);


	// ---------------------------------------------------------------------------------------------------------------------
	// Enable write protection
	Print(L"[INFO]  Hooks01LoadImage.c (HookServiceTablePointer) - Enable write protection\r\n");

	FunctionLocal_UtilsRegisters_EnableWriteProtection(WpEnabled);


	// ---------------------------------------------------------------------------------------------------------------------
	// Restore TPL
	Print(L"[INFO]  Hooks01LoadImage.c (HookServiceTablePointer) - Restore TPL\r\n");

	// Restores a task’s priority level to its previous value.
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-restoretpl
	gBS->RestoreTPL(Tpl);


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	Print(L"[INFO]  Hooks01LoadImage.c (HookServiceTablePointer) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return OriginalFunction;
}



/**
	@brief          Hooks and processes the Boot Service's LoadImage function.
	@details        This function serves as a custom hook for UEFI's LoadImage function. It performs additional processing before calling the original LoadImage function.


	@param[in]      BootPolicy                  Indicates the policy for loading the image. If TRUE, the image is loaded as a boot option; otherwise, it is loaded without boot option.
	@param[in]      ParentImageHandle           The handle of the image that is loading this image.
	@param[in]      DevicePath                  The pointer to the device path of the image.
	@param[in]      SourceBuffer                If not NULL, a pointer to the memory location containing a copy of the image to be loaded.
	@param[in]      SourceSize                  The size in bytes of SourceBuffer.
	@param[out]     ImageHandle                 The pointer where the handle of the loaded image will be returned.


	@retval         EFI_SUCCESS                 The image is successfully loaded.
	@retval         other                       An error occurred.
**/
EFI_STATUS
FunctionLocal_HooksLoadImage_HookgBSLoadImage(
	IN              BOOLEAN                     BootPolicy,
	IN              EFI_HANDLE                  ParentImageHandle,
	IN              EFI_DEVICE_PATH_PROTOCOL    *DevicePath,
	IN              VOID                        *SourceBuffer OPTIONAL,
	IN              UINTN                       SourceSize,
	OUT             EFI_HANDLE                  *ImageHandle
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	EFI_STATUS Status;
	EFI_LOADED_IMAGE_PROTOCOL *LoadedImage = NULL;


	// ---------------------------------------------------------------------------------------------------------------------
	// Call original LoadImage function
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - Call original LoadImage function\r\n");

	Status = Global_HooksLoadImage_OriginalBootServicesLoadImage(BootPolicy, ParentImageHandle, DevicePath, SourceBuffer, SourceSize, ImageHandle);

	// Failed
	if (EFI_ERROR(Status) || *ImageHandle == NULL)
	{
		Print(L"[ERROR] Hooks01LoadImage.c (HookgBSLoadImage) - Global_HooksLoadImage_OriginalBootServicesLoadImage failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get loaded image info
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - Get loaded image info\r\n");

	// Queries a handle to determine if it supports a specified protocol. If the protocol is supported by the handle, it opens the protocol on behalf of the calling agent. This is an extended version of the EFI boot service EFI_BOOT_SERVICES.HandleProtocol().
	// https://uefi.org/specs/UEFI/2.11/07_Services_Boot_Services.html#efi-boot-services-openprotocol
	Status = gBS->OpenProtocol(*ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
	LoadedImage->ParentHandle = NULL;

	// Failed
	if (EFI_ERROR(Status))
	{
		Print(L"[ERROR] Hooks01LoadImage.c (HookgBSLoadImage) - gBS->OpenProtocol [gEfiLoadedImageProtocolGuid] failed: %r (%llx).\r\n", Status, Status);
		return Status;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Image info
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - Image info\r\n");
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - FilePath = %llx\r\n", LoadedImage->FilePath);
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - Text = %s\r\n", ConvertDevicePathToText(LoadedImage->FilePath, TRUE, TRUE));
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - ImageBase = %llx\r\n", LoadedImage->ImageBase);
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - ImageSize = %llx\r\n", LoadedImage->ImageSize);


	// ---------------------------------------------------------------------------------------------------------------------
	// Image loaded in memory
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - The bootloader image has been successfully loaded into memory. At this stage, execution has not yet been transferred to the image entry point. This means we can safely inspect or hook functions inside the image (e.g., Windows bootmgfw.efi or Linux grubx64.efi) before handing over control.\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	Print(L"[INFO]  Hooks01LoadImage.c (HookgBSLoadImage) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return EFI_SUCCESS;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

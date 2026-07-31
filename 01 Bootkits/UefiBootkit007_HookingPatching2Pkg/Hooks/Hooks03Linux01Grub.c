// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks Linux Grub Efi
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



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Custom hook for grub_efi_get_secureboot.
	@details        This function replaces the original grub_efi_get_secureboot implementation. In GRUB, grub_efi_get_secureboot determines whether UEFI Secure Boot is enabled by querying several EFI variables such as 'SecureBoot', 'SetupMode', and 'MokSBState'. For demonstration purposes, our hook ignores all real checks and simply forces a predefined return value. This allows us to verify that: 1 the hook mechanism works correctly and 2 we can alter the behavior of arbitrary GRUB functions in memory. In a real attack scenario, forcing Secure Boot to appear 'Disabled' (value 2) would allow unsigned code to run without restrictions. Here we invert the logic and force it to always report 'Enabled' (value 3), which is harmless but proves the concept.


	@return         UINT8                       1 Unknown (fallback), 2 Disabled, 3 Enabled
**/
UINT8
FunctionLocal_HooksLinuxGrub_GrubEfiGetSecureBoot()
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  Hooks03Linux01Grub.c (GrubEfiGetSecureBoot) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Force Secure Boot to Enabled. Return value '3' corresponds to 'Enabled' state. This overrides the real logic that checks EFI variables.
	Print(L"INFO]  Hooks03Linux01Grub.c (GrubEfiGetSecureBoot) - Forcing Secure Boot state.\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return (UINT8)3;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

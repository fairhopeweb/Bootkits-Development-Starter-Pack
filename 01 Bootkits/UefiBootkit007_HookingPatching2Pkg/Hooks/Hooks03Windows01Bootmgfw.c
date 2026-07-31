// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks Windows Bootmgfw Efi
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Custom hook for SbIsEnabled2.
	@details        This function replaces the original SbIsEnabled2 implementation inside bootmgfw.efi. In the Windows boot process, bootmgfw.efi calls SbIsEnabled2 to determine whether UEFI Secure Boot is currently enabled by querying the platform firmware state and writing the result into an output buffer. For demonstration purposes, our hook ignores all real checks and simply forces a predefined return value. This allows us to verify that: 1 the hook mechanism works correctly and 2 we can alter the behavior of arbitrary bootmgfw functions in memory. In a real attack scenario, forcing Secure Boot to appear 'Disabled' (value 0) would allow unsigned code to load without restrictions. Here we invert the logic and force it to always report 'Enabled' (value 1), which is harmless but proves the concept.


	@param[in]      DummyInput                  Unused parameter preserved to match the original function signature.
	@param[out]     SecureBootState             Pointer to a UINT8 buffer where the Secure Boot state is written. A value of 1 indicates Enabled.


	@return         INT64                       0 on success, matching the original function return convention.
**/
INT64
FunctionLocal_HooksWindowsBootmgfw_SbIsEnabled2(
	IN              INT32                       DummyInput,
	OUT             UINT8                       *SecureBootState
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	Print(L"[INFO]  Hooks03Windows01Bootmgfw.c (SbIsEnabled2) - Hello\r\n");


	// ------------------------------------------------------------------------
	// Debug log to confirm the hook is triggered
	// ------------------------------------------------------------------------
	Print(L"[INFO]  Hooks03Windows01Bootmgfw.c (SbIsEnabled2) - SbIsEnabled2 called, forcing Secure Boot enabled\n");


	// ------------------------------------------------------------------------
	// Always force Secure Boot state = 1 (enabled)
	// ------------------------------------------------------------------------
	if (SecureBootState != NULL) {
		*SecureBootState = 1;
	}


	// ------------------------------------------------------------------------
	// Return 0, same as the original function did
	// ------------------------------------------------------------------------
	return 0;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

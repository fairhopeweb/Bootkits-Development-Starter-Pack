// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Registers
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------
// START -> LOCAL -----------------------------------------------------------------------------------------------------------------------------



#include <Utils/Utils01Registers.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Disable WP if supported and retrieve their current state.
	@details        This function disables WP (Write Protection) if it is supported by the system's hardware. It also retrieves the current state of this feature and stores it in the output parameter.


	@param[out]     WpEnabled                   A pointer to a boolean variable to store the current write protection state. True if write protection is currently enabled, False otherwise.


	@retval         None
**/
VOID
FunctionLocal_UtilsRegisters_DisableWriteProtection(
	OUT             BOOLEAN                     *WpEnabled
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//Print(L"[INFO]  UtilsRegisters.c (DisableWriteProtection) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables

	// Reads the current value of the Control Register 0 (CR0). Reads and returns the current value of CR0. This function is only available on IA-32 and x64. This returns a 32-bit value on IA-32 and a 64-bit value on x64.
	// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr0.c#L21
	CONST UINTN Cr0 = AsmReadCr0();
	*WpEnabled = (Cr0 & CR0_WP) != 0;


	// ---------------------------------------------------------------------------------------------------------------------
	// Disable Write Protection
	//Print(L"[INFO]  UtilsRegisters.c (DisableWriteProtection) - Disable Write Protection\r\n");

	if (*WpEnabled)
	{
		//Print(L"[INFO]  UtilsRegisters.c (DisableWriteProtection) - AsmWriteCr0\r\n");

		// Writes a value to Control Register 0 (CR0). Writes and returns a new value to CR0. This function is only available on IA-32 and x64. This writes a 32-bit value on IA-32 and a 64-bit value on x64.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/WriteCr0.c#L22
		AsmWriteCr0(Cr0 & ~CR0_WP);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//Print(L"[INFO]  UtilsRegisters.c (DisableWriteProtection) - Bye\r\n");
}



/**
	@brief          Enable WP if supported.
	@details        This function enables WP (Write Protection) if it is supported by the system's hardware. It takes one boolean parameter to control the enabling/disabling of this feature.


	@param[in]      WpEnabled                   A boolean indicating whether write protection should be enabled.


	@retval         None
**/
VOID
FunctionLocal_UtilsRegisters_EnableWriteProtection(
	IN              BOOLEAN                     WpEnabled
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//Print(L"[INFO]  UtilsRegisters.c (EnableWriteProtection) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Enable write protection
	//Print(L"[INFO]  UtilsRegisters.c (EnableWriteProtection) - Enable write protection\r\n");

	if (WpEnabled)
	{
		//Print(L"[INFO]  UtilsRegisters.c (EnableWriteProtection) - AsmWriteCr0\r\n");

		// Reads the current value of the Control Register 0 (CR0). Reads and returns the current value of CR0. This function is only available on IA-32 and x64. This returns a 32-bit value on IA-32 and a 64-bit value on x64.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/ReadCr0.c#L21

		// Writes a value to Control Register 0 (CR0). Writes and returns a new value to CR0. This function is only available on IA-32 and x64. This writes a 32-bit value on IA-32 and a 64-bit value on x64.
		// https://github.com/tianocore/edk2/blob/master/MdePkg/Library/BaseLib/Ia32/WriteCr0.c#L22
		AsmWriteCr0(AsmReadCr0() | CR0_WP);
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//Print(L"[INFO]  UtilsRegisters.c (EnableWriteProtection) - Bye\r\n");
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

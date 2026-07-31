// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Address
**/



// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------
// START -> LIBRARIES -------------------------------------------------------------------------------------------------------------------------



// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Uefi.h
// Root include file for Mde Package UEFI, UEFI_APPLICATION type modules. This is the include file for any module of type UEFI and UEFI_APPLICATION. Uefi modules only use types defined via this include file and can be ported easily to any environment.
#include <Uefi.h>

// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Library/UefiLib.h
// Provides library functions for common UEFI operations. Only available to DXE and UEFI module types. The UEFI Library provides functions and macros that simplify the development of UEFI Drivers and UEFI Applications. These functions and macros help manage EFI events, build simple locks utilizing EFI Task Priority Levels (TPLs), install EFI Driver Model related protocols, manage Unicode string tables for UEFI Drivers, and print messages on the console output and standard error devices.
#include <Library/UefiLib.h>



// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------
// START -> STRUCTURES ------------------------------------------------------------------------------------------------------------------------



#include <Structures/Structures01PortableExecutable.h>



// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------
// START -> FUNCTIONS -------------------------------------------------------------------------------------------------------------------------



/**
	@brief          Locate the start address of a function in a Linux GRUB (ELF/EFI) binary.
	@details        Unlike Windows PE images (which provide unwind/exception tables to locate function boundaries), Linux GRUB binaries (ELF/EFI) compiled with CET (Control-flow Enforcement Technology) use the endbr64 instruction at the entry point of each function. The endbr64 instruction (opcode: F3 0F 1E FA) is emitted by compilers to support Intel CET's Indirect Branch Tracking (IBT). It acts as a marker that must appear at the beginning of every valid function entry point, so that indirect calls/jumps can only target legitimate entries. This function scans backwards in memory from a known address inside a function until it finds the nearest endbr64 marker. The address of that marker is returned as the function's start address. A safety limit is imposed to avoid scanning arbitrarily large regions or crossing outside the code section.


	@param[in]      ImageBase                   The base address of the PE image.
	@param[in]      NtHeaders                   A pointer to the NT Headers of the PE image (unused for Linux/GRUB, kept for compatibility).
	@param[in]      AddressInFunction           An address somewhere inside the function whose start is to be located.


	@retval         UINT8*                      Returns a pointer to the start address of the function, or NULL if the function start address could not be determined.
**/
UINT8*
FunctionLocal_UtilsAddress_FindStartAddressLinux(
	IN              CONST UINT8*                ImageBase,
	IN              PEFI_IMAGE_NT_HEADERS       NtHeaders,
	IN              CONST UINT8*                AddressInFunction
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressLinux) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if AddressInFunction pointer is null
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressLinux) - Check if AddressInFunction pointer is null\r\n");

	// Failed
	if (AddressInFunction == NULL)
	{
		Print(L"[ERROR] UtilsAddress.c (FindStartAddressLinux) - Check [Check if AddressInFunction pointer is null].\r\n");
		return NULL;
	}

	// ---------------------------------------------------------------------------------------------------------------------
	// Show the provided pointer
	Print(L"[INFO]  UtilsAddress.c (FindStartAddressLinux) - Input address: %p (offset %lx)\r\n", AddressInFunction, (UINTN)(AddressInFunction - ImageBase));


	// ---------------------------------------------------------------------------------------------------------------------
	// Variables
	CONST UINTN MaxBackwardsScan = 0x2000;


	// ---------------------------------------------------------------------------------------------------------------------
	// Start backwards scan
	Print(L"[INFO]  UtilsAddress.c (FindStartAddressLinux) - Start backwards scan (limit: %u bytes)\r\n", MaxBackwardsScan);

	for (UINTN i = 0; i < MaxBackwardsScan; i++)
	{
		CONST UINT8* candidate = AddressInFunction - i;

		// -----------------------------------------------------------------------------------------------------------------
		// Check for endbr64 (F3 0F 1E FA)
		if (candidate[0] == 0xF3 && candidate[1] == 0x0F && candidate[2] == 0x1E && candidate[3] == 0xFA)
		{

			// -------------------------------------------------------------------------------------------------------------
			// Match found
			Print(L"[INFO]  UtilsAddress.c (FindStartAddressLinux) - Found function entry!\r\n");
			Print(L"[INFO]  UtilsAddress.c (FindStartAddressLinux) - Candidate address: %p (offset %lx)\r\n", candidate, (UINTN)(candidate - ImageBase));
			Print(L"[INFO]  UtilsAddress.c (FindStartAddressLinux) - Found function start at offset %lx\r\n", (UINTN)(candidate - ImageBase));


			// -------------------------------------------------------------------------------------------------------------
			// Return
			return (UINT8*)candidate;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Match not found
	Print(L"[ERROR] UtilsAddress.c (FindStartAddressLinux) - No function start found near %lx\r\n", (UINTN)(AddressInFunction - ImageBase));


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return NULL;
}



/**
	@brief          Locate the start address of a function in a Windows PE/EFI image.
	@details        Unlike Linux GRUB (ELF/EFI) binaries that rely on the endbr64 marker, Windows PE images provide structured unwind/exception data that describes each function's boundaries. The PE Optional Header contains a data directory entry for the exception table, which is an array of IMAGE_RUNTIME_FUNCTION_ENTRY structures. Each entry defines the BeginAddress and EndAddress of a function, along with unwind metadata. This function performs a binary search over the exception table to find which function contains the provided address. Once found, it returns the start address of that function relative to the loaded image base. If the entry is marked as "indirect", the function resolves it accordingly. If no valid function entry is found, the routine returns NULL.  


	@param[in]      ImageBase                   The base address of the PE image.
	@param[in]      NtHeaders                   A pointer to the NT Headers of the PE image (used to locate the exception table).
	@param[in]      AddressInFunction           An address somewhere inside the function whose start is to be located.


	@retval         UINT8*                      Returns a pointer to the start address of the function, or NULL if the function start address could not be determined.
**/
UINT8*
FunctionLocal_UtilsAddress_FindStartAddressWindows(
	IN              CONST UINT8*                ImageBase,
	IN              PEFI_IMAGE_NT_HEADERS       NtHeaders,
	IN              CONST UINT8*                AddressInFunction
)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// Hello
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Hello\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if AddressInFunction pointer is null
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Check if AddressInFunction pointer is null\r\n");

	// Failed
	if (AddressInFunction == NULL)
	{
		Print(L"[ERROR] UtilsAddress.c (FindStartAddressWindows) - Check [Check if AddressInFunction pointer is null].\r\n");
		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if the image has exception data directory
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Check if the image has exception data directory\r\n");

	// Failed
	if (NtHeaders->OptionalHeader.NumberOfRvaAndSizes <= EFI_IMAGE_DIRECTORY_ENTRY_EXCEPTION)
	{
		Print(L"[ERROR] UtilsAddress.c (FindStartAddressWindows) - Check [Check if the image has exception data directory].\r\n");
		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get function table size
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Get function table size\r\n");

	CONST UINT32 FunctionTableSize = NtHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size;


	// ---------------------------------------------------------------------------------------------------------------------
	// Check if function table size is zero
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Check if function table size is zero\r\n");

	// Failed
	if (FunctionTableSize == 0)
	{
		Print(L"[ERROR] UtilsAddress.c (FindStartAddressWindows) - Check [Check if function table size is zero].\r\n");
		return NULL;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Get function table
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Get function table\r\n");

	CONST PIMAGE_RUNTIME_FUNCTION_ENTRY FunctionTable = (PIMAGE_RUNTIME_FUNCTION_ENTRY)(ImageBase + NtHeaders->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);


	// ---------------------------------------------------------------------------------------------------------------------
	// Calculate relative address to perform binary search
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Calculate relative address to perform binary search\r\n");

	CONST UINT32 RelativeAddress = (UINT32)(AddressInFunction - ImageBase);
	PIMAGE_RUNTIME_FUNCTION_ENTRY FunctionEntry = NULL;
	INT32 Low = 0;
	INT32 High = (INT32)(FunctionTableSize / sizeof(IMAGE_RUNTIME_FUNCTION_ENTRY)) - 1;


	// ---------------------------------------------------------------------------------------------------------------------
	// Find the function that contains the specified address
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Find the function that contains the specified address\r\n");

	// Loop
	while (High >= Low)
	{
		CONST INT32 Middle = (Low + High) >> 1;
		FunctionEntry = &FunctionTable[Middle];
		// Check if address is within current function entry's range
		if (RelativeAddress < FunctionEntry->BeginAddress)
		{
			High = Middle - 1;
		}
		else if (RelativeAddress >= FunctionEntry->EndAddress)
		{
			Low = Middle + 1;
		}
		else
		{
			break;
		}
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Search valid function entry
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Search valid function entry\r\n");

	if (High >= Low)
	{
		// Function entry marked as indirect
		if ((FunctionEntry->u.UnwindData & RUNTIME_FUNCTION_INDIRECT) != 0)
		{
			FunctionEntry = (PIMAGE_RUNTIME_FUNCTION_ENTRY)(FunctionEntry->u.UnwindData + ImageBase - 1);
		}

		// Match found
		//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Match found\r\n");

		// Retrieve the address where the function begins
		//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Retrieve the address where the function begins\r\n");

		return (UINT8*)ImageBase + FunctionEntry->BeginAddress;
	}


	// ---------------------------------------------------------------------------------------------------------------------
	// Match not found
	Print(L"[ERROR] UtilsAddress.c (FindStartAddressWindows) - Match not found.\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Bye
	//Print(L"[INFO]  UtilsAddress.c (FindStartAddressWindows) - Bye\r\n");


	// ---------------------------------------------------------------------------------------------------------------------
	// Return
	return NULL;
}



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

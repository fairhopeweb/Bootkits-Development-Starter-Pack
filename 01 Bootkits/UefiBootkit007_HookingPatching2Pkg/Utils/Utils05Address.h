// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Address
**/



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
);



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

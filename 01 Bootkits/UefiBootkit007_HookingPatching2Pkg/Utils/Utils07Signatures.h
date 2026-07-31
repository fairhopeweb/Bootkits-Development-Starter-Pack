// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Signatures
**/



// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------
// START -> GLOBAL VARIABLES ------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------------------------------
// GRUBX64.EFI
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	Signature for [grubx64]grub_efi_get_secureboot

	grub_efi_get_secureboot is responsible for querying UEFI firmware variables (SecureBoot, SetupMode, MokSBState) to determine whether UEFI Secure Boot is enabled or disabled. This function plays a critical role in enforcing secure boot validation when loading kernels or chainloading bootloaders.

	Signature bytes:
		55 31 D2 31 C0 48 89 E5 41 54

	Instructions:
		push rbp
		xor edx, edx
		xor eax, eax
		mov rbp, rsp
		push r12

	References:
		grub-core/kern/efi/sb.c

	This byte pattern marks the function prologue for grub_efi_get_secureboot inside grubx64.efi.

	To locate and verify this:

	1. Build GRUB with UEFI x86_64 target:
		- ./configure --with-platform=efi --target=x86_64
		- make -j$(nproc)

	2. Locate the core binary for inspection:
		- grub-core/kernel.exec (contains symbols and full function names)
		- grubx64.efi (stripped executable EFI loader)

	3. Import the binary into Ghidra or IDA Pro as a PE file (64-bit).

	4. Search for the byte pattern or the embedded string inside IDA Pro:

			Option A - Sequence of bytes (Alt + B):
				- Open the Search menu and select Sequence of bytes...
				- In the search dialog:
					Enter the pattern: 55 31 D2 31 C0 48 89 E5 41 54
					Select direction: Down
					Check: Find all occurrences
					Check: Match case
					Check: Hex
				- Click OK and review the results window

			Option B - String search (Alt + T):
				- Open the Search menu and select Text...
				- In the search dialog:
					Enter the string: UEFI Secure Boot state: or the string SetupMode
					Select direction: Down
					Check: Find all occurrences
					Check: Match case
				- Click OK and review the results window

	5. Focus on the first occurrence with this prologue. It corresponds to grub_efi_get_secureboot. Hooking or patching this function allows control over the Secure Boot state reported by GRUB to downstream loaders.
**/
STATIC CONST UINT8 Global_UtilsSignatures_Signature_Grubx64_GrubEfiGetSecureBoot[] = {
	0x55, 0x31, 0xD2, 0x31, 0xC0, 0x48, 0x89, 0xE5, 0x41, 0x54
};



// --------------------------------------------------------------------------------------------------------------------------------------------
// BOOTMGFW.EFI
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	Signature for [bootmgfw]SbIsEnabled2

	SbIsEnabled2 is invoked inside the Windows Boot Manager (bootmgfw.efi) to determine whether Secure Boot is enabled. Internally, it queries the EFI variables SetupMode and SecureBoot using EfiGetVariable and caches the result. The final output is stored in the second parameter (a pointer), which is set to 1 if Secure Boot is enabled.

	Signature bytes:
		48 8B C4 89 48 08 53

	Instructions:
		mov rax, rsp
		mov [rax+8], ecx
		push rbx

	References:
		Disassembly of bootmgfw.efi

	This byte pattern can appear in multiple places within bootmgfw.efi. However, we always hook only the first match found (the one located at the lowest memory address). This first occurrence reliably corresponds to SbIsEnabled2 across supported Windows versions.

	To locate and verify this:

	1. Mount the EFI System Partition (ESP):
		- mountvol.exe U: /s
	
	2. Copy the Windows Boot Manager to your user's Downloads folder:
		- copy U:\EFI\Microsoft\Boot\bootmgfw.efi %USERPROFILE%\Downloads\bootmgfw.efi

	3. Unmount the EFI System Partition (ESP):
		- mountvol.exe U: /d

	4. Import the binary into Ghidra or IDA Pro as a PE file (64-bit).

	5. Search for the byte pattern or the embedded string inside IDA Pro:

			Option A - Sequence of bytes (Alt + B):
				- Open the Search menu and select Sequence of bytes...
				- In the search dialog:
					Enter the pattern: 48 8B C4 89 48 08 53
					Select direction: Down
					Check: Find all occurrences
					Check: Match case
					Check: Hex
				- Click OK and review the results window

			Option B - String search (Alt + T):
				- Open the Search menu and select Text...
				- In the search dialog:
					Enter the string: SetupMode or the string SecureBoot
					Select direction: Down
					Check: Find all occurrences
					Check: Match case
				- Click OK and review the results window

	6. Among the results, focus on the first occurrence (the one with the lowest memory address), as it corresponds to SbIsEnabled2 and is the one we hook.
**/
STATIC CONST UINT8 Global_UtilsSignatures_Signature_SbIsEnabled2[] = {
	0x48, 0x8B, 0xC4, 0x89, 0x48, 0x08, 0x53
};



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

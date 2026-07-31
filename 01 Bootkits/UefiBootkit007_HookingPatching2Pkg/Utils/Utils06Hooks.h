// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Utils Hooks
**/



// --------------------------------------------------------------------------------------------------------------------------------------------
// HOOKS
// --------------------------------------------------------------------------------------------------------------------------------------------


/**
	Hook Template

	A "faux call" is a technique used in low-level programming and reverse engineering to divert or redirect a program's execution to a custom function or code snippet.
	
	The technique involves inserting machine instructions into a program's execution flow to force it to execute specific code at a particular point, rather than continuing with its normal flow. This is achieved by manipulating existing machine instructions or inserting new instructions that perform custom tasks.

	48 B8
	00 00 00 00 00 00 00 00
	50
	C3

	https://github.com/Mattiwatti/EfiGuard/blob/master/EfiGuardDxe/PatchBootmgr.c
**/
static CONST UINT8 Global_UtilsHooks_FauxCallHookTemplate[] = {
	0x48, 0xB8,												// mov rax,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			// <addr>
	0x50,													// push [e|r]ax
	0xC3													// ret
};

// Offset
static CONST UINTN Global_UtilsHooks_FauxCallHookTemplateAddressOffset = 2;



// --------------------------------------------------------------------------------------------------------------------------------------------
// ORIGINAL FUNCTIONS
// --------------------------------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------------------------------
// UEFI
// --------------------------------------------------------------------------------------------------------------------------------------------



// gBS->LoadImage()
extern EFI_IMAGE_LOAD Global_UtilsHooks_OriginalFunction_BootServicesLoadImage;



// --------------------------------------------------------------------------------------------------------------------------------------------
// GRUBX64.EFI
// --------------------------------------------------------------------------------------------------------------------------------------------



// grubx64.efi->grub_efi_get_secureboot()
typedef
UINT8
(*Global_UtilsHooks_OriginalFunction_GrubEfiGetSecureBoot)();
extern VOID* Global_UtilsHooks_OriginalPointer_GrubEfiGetSecureBoot;
extern UINT8 Global_UtilsHooks_BackupBytes_GrubEfiGetSecureBoot[sizeof(Global_UtilsHooks_FauxCallHookTemplate)];



// --------------------------------------------------------------------------------------------------------------------------------------------
// BOOTMGFW.EFI
// --------------------------------------------------------------------------------------------------------------------------------------------



// bootmgfw.efi->SbIsEnabled2()
typedef
INTN
(*Global_UtilsHooks_OriginalFunction_SbIsEnabled2)
(
	IN				UINTN						CallerContext,
	OUT				UINT8*						SecureBootFlag
);
extern VOID* Global_UtilsHooks_OriginalPointer_SbIsEnabled2;
extern UINT8 Global_UtilsHooks_BackupBytes_SbIsEnabled2[sizeof(Global_UtilsHooks_FauxCallHookTemplate)];



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks Boot
**/



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

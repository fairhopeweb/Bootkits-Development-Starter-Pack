// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------



/**
	@file

	Hooks Windows Bootmgfw Efi
**/



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
);



// --------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

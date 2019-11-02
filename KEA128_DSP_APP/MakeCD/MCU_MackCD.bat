@echo off
del KEA128_BOOT_APP.hex
copy %~d0\project\DSP\KEA128_DSD\KEA128_DSP_BOOT\Keil_Pro\Output\*.hex
ren KEA128.hex KEA128_BOOT.hex
copy %~d0\project\DSP\KEA128_DSD\KEA128_DSP_APP\Keil_Pro\Output\*.hex
ren KEA128.hex KEA128_APP.hex
SetLocal EnableDelayedExpansion
for /f %%i in (KEA128_BOOT.hex) do (
	set index=%%i
	if not "!index:~0,9!"==":00000001" (
		if not "!index:~0,9!"==":04000005" (
			echo %%i>>KEA128_BOOT_APP.hex
		)			
	)
)
for /f %%i in (KEA128_APP.hex) do ( 
	echo %%i>>KEA128_BOOT_APP.hex
)
del KEA128_BOOT.hex KEA128_APP.hex
del mcu.bin
copy %~d0\project\DSP\KEA128_DSD\KEA128_DSP_APP\Keil_Pro\BIN\*.bin
ren KEA128.bin mcu.bin
start MCU_MackCD.exe mcu.bin mcu_v_kea.img
@pause
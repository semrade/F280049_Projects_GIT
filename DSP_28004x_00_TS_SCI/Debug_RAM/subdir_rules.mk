################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-c2000_20.2.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --opt_for_speed=0 --fp_mode=relaxed --include_path="C:/Users/STarikUser/Desktop/F280049_Projects/DSP_280049_Common/driverlib/f28004x/driverlib/inc" --include_path="C:/Users/STarikUser/Desktop/F280049_Projects/DSP_280049_Common/driverlib/f28004x/driverlib" --include_path="C:/Users/STarikUser/Desktop/F280049_Projects/DSP_280049_Common/f28004x/common/include" --include_path="C:/Users/STarikUser/Desktop/F280049_Projects/DSP_280049_Common/f28004x/headers/include" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-c2000_20.2.2.LTS/include" --advice:performance=all --define=_LAUNCHXL_F280049C --define=_DUAL_HEADERS -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



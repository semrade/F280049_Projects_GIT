################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
TS_SysMng/Src/%.obj: ../TS_SysMng/Src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-c2000_20.2.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --opt_for_speed=0 --include_path="C:/Users/STarikUser/Documents/F280049_Projects/DSP_280049_Common/f28004x/headers/include" --include_path="C:/Users/STarikUser/Documents/F280049_Projects/DSP_280049_Common/f28004x/common/include" --include_path="C:/Users/STarikUser/Documents/F280049_Projects/DSP_280049_Common/driverlib/f28004x/driverlib" --include_path="C:/Users/STarikUser/Documents/F280049_Projects/DSP_280049_Common/driverlib/f28004x/driverlib/inc" --include_path="C:/Users/STarikUser/Documents/F280049_Projects/DSP_28004x_01_TS_CAN/TS_SysMng/Inc" --include_path="C:/Users/STarikUser/Documents/F280049_Projects/DSP_28004x_01_TS_CAN" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-c2000_20.2.2.LTS/include" --advice:performance=none --define=_DUAL_HEADERS --define=_LAUNCHXL_F280049C -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="TS_SysMng/Src/$(basename $(<F)).d_raw" --obj_directory="TS_SysMng/Src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



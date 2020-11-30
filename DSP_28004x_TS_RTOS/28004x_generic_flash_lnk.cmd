
MEMORY
{
PAGE 0 :  /* Program Memory */
          /* BEGIN is used for the "boot to FLASH" bootloader mode   */

    D01SARAM   : origin = 0x00B000, length = 0x001000

    /* Flash boot address */
    BEGIN   : origin = 0x080000, length = 0x000002
    /* Flash sectors */
    FLASHA  : origin = 0x080002, length = 0x00FFFE  /* on-chip Flash */
    FLASHB  : origin = 0x090000, length = 0x010000  /* on-chip Flash */
    RESET   : origin = 0x3FFFC0, length = 0x000002

PAGE 1 : /* Data Memory */

    BOOT_RSVD : origin = 0x000002, length = 0x000120 /* Part of M0, BOOT rom
                                                        will use this for
                                                        stack */

    M01SARAM : origin = 0x000122, length = 0x0006DE  /* on-chip RAM */

    LS05SARAM : origin = 0x008000, length = 0x004000 /* on-chip RAM */

    /* on-chip Global shared RAMs */
    RAMGS0  : origin = 0x00C000, length = 0x002000
    RAMGS1  : origin = 0x00E000, length = 0x002000
    RAMGS2  : origin = 0x010000, length = 0x002000
    RAMGS3  : origin = 0x012000, length = 0x002000
}


SECTIONS
{
   codestart        : > BEGIN,     PAGE = 0
   .text            : >>FLASHA | FLASHB   PAGE = 0, ALIGN(4)
   .cinit           : > FLASHA | FLASHB      PAGE = 0, ALIGN(4)
   .switch          : > FLASHA | FLASHB      PAGE = 0, ALIGN(4)
   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */

   .stack           : > M01SARAM | LS05SARAM ,     PAGE = 1

#if defined(__TI_EABI__)
   .init_array      : > FLASHA | FLASHB,       PAGE = 0,       ALIGN(4)
   .bss             : > M01SARAM | LS05SARAM       PAGE = 1
   .bss:output      : > M01SARAM | LS05SARAM       PAGE = 0
   .bss:cio         : > M01SARAM | LS05SARAM       PAGE = 0
   .data            : > M01SARAM | LS05SARAM | RAMGS0 | RAMGS1      PAGE = 1
   .sysmem          : > LS05SARAM | M01SARAM,       PAGE = 1
   /* Initalized sections go in Flash */
   .const           : > FLASHA | FLASHB,       PAGE = 0,       ALIGN(4)
#else
   .pinit           : > FLASHA | FLASHB,       PAGE = 0,       ALIGN(4)
   .ebss            : > M01SARAM | LS05SARAM,       PAGE = 1
   .esysmem         : > LS05SARAM | M01SARAM,       PAGE = 1
   .cio             : > LS05SARAM | M01SARAM,       PAGE = 1
   .econst          : > FLASHA | FLASHB,    PAGE = 0, ALIGN(4)
#endif

   ramgs0           : > RAMGS0,    PAGE = 1
   ramgs1           : > RAMGS1,    PAGE = 1

 
#if defined(__TI_EABI__) 
   .TI.ramfunc      : LOAD = FLASHA | FLASHB PAGE = 0,
                      RUN = LS05SARAM PAGE = 1,
                      LOAD_START(RamfuncsLoadStart),
                      LOAD_SIZE(RamfuncsLoadSize),
                      LOAD_END(RamfuncsLoadEnd),
                      RUN_START(RamfuncsRunStart),
                      RUN_SIZE(RamfuncsRunSize),
                      RUN_END(RamfuncsRunEnd),
                      ALIGN(4)
#else					  
   .TI.ramfunc      : LOAD = FLASHA | FLASHB PAGE = 0,
                      RUN = LS05SARAM PAGE = 1,
                      LOAD_START(_RamfuncsLoadStart),
                      LOAD_SIZE(_RamfuncsLoadSize),
                      LOAD_END(_RamfuncsLoadEnd),
                      RUN_START(_RamfuncsRunStart),
                      RUN_SIZE(_RamfuncsRunSize),
                      RUN_END(_RamfuncsRunEnd),
                      ALIGN(4)
#endif

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/

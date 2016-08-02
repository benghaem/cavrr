set(AVR_COMPILER "avr-gcc")
set(AVR_OBJCOPY "avr-objcopy")
set(AVR_OBJDUMP "avr-objdump")
set(AVR_MMCU "attiny45")

list(APPEND programs "simpleoutput")
list(APPEND programs "simpleoutput_plusdelay")
list(APPEND programs "simplestatemachine")

if(COMPILE_IHEX)
    foreach(program ${programs})
        add_custom_command(
            OUTPUT     ${CMAKE_CURRENT_BINARY_DIR}/${program}.o
            DEPENDS    ${CMAKE_CURRENT_SOURCE_DIR}/src/${program}.c
            COMMAND    ${AVR_COMPILER}
            ARGS       -g -mmcu=${AVR_MMCU} -c ${CMAKE_CURRENT_SOURCE_DIR}/src/${program}.c
            COMMENT     "Compiling"
        )
        add_custom_command(
            OUTPUT     ${CMAKE_CURRENT_BINARY_DIR}/${program}.elf
            DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/${program}.o
            COMMAND    ${AVR_COMPILER}
            ARGS       -g -mmcu=${AVR_MMCU} -o ${CMAKE_CURRENT_BINARY_DIR}/${program}.elf ${CMAKE_CURRENT_BINARY_DIR}/${program}.o
            COMMENT    "Linking"
        )
        add_custom_command(
            OUTPUT     ${CMAKE_CURRENT_BINARY_DIR}/${program}.lst
            DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/${program}.elf
            COMMAND    ${AVR_OBJDUMP}
            ARGS       -h -S ${CMAKE_CURRENT_BINARY_DIR}/${program}.elf > ${CMAKE_CURRENT_BINARY_DIR}/${program}.lst
            COMMENT    "Dump object"
        )
        add_custom_command(
            OUTPUT     ${CMAKE_CURRENT_BINARY_DIR}/${program}.hex
            DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/${program}.elf
            COMMAND    ${AVR_OBJCOPY}
            ARGS       -j .text -j .data -O ihex ${CMAKE_CURRENT_BINARY_DIR}/${program}.elf ${CMAKE_CURRENT_BINARY_DIR}/${program}.hex
            COMMENT    "Output ihex"
        )
        add_custom_command(
            OUTPUT     ${CMAKE_CURRENT_BINARY_DIR}/${program}.epr
            DEPENDS    ${CMAKE_CURRENT_BINARY_DIR}/${program}.elf
            COMMAND    ${AVR_OBJCOPY}
            ARGS       -j .eeprom --change-section-lma .eeprom=0 -O ihex ${CMAKE_CURRENT_BINARY_DIR}/${program}.elf ${CMAKE_CURRENT_BINARY_DIR}/${program}.epr
            COMMENT    "Output eeprom"
        )
        ADD_CUSTOM_TARGET(IHEX_${program} ALL echo
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${program}.epr ${CMAKE_CURRENT_BINARY_DIR}/${program}.hex ${CMAKE_CURRENT_BINARY_DIR}/${program}.lst
        )
    endforeach(program)
endif(COMPILE_IHEX)


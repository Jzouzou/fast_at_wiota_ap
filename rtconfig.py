import os
ARCH     = 'risc-v'
CPU      = 'uc8088'
# toolchains options
CROSS_TOOL  = 'gcc'

#------- toolchains path -------------------------------------------------------
if os.getenv('RTT_CC'):
    CROSS_TOOL = os.getenv('RTT_CC')

if  CROSS_TOOL == 'gcc':
    PLATFORM    = 'gcc'
    EXEC_PATH   = '/opt/riscv/bin/'
else:
    print('Please make sure your toolchains is GNU GCC!')
    exit(0)

# if os.getenv('RTT_EXEC_PATH'):
#     EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD = 'debug'
#BUILD = 'release'

CORE = 'risc-v'
MAP_FILE = 'rtthread.map'
LINK_FILE = './libraries/link.flash.ld'
TARGET_NAME = 'rtthread.bin'

#------- GCC settings ----------------------------------------------------------
if PLATFORM == 'gcc':
    # toolchains
    #PREFIX = 'riscv-none-embed-'
    PREFIX = 'riscv32-unknown-elf-'
    CC = PREFIX + 'gcc'
    CXX= PREFIX + 'g++'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'elf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'

    #DEVICE = ' -march=rv32imac -mabi=ilp32 -DUSE_PLIC -DUSE_M_TIME -DNO_INIT -mcmodel=medany -msmall-data-limit=8 -L.  -nostartfiles  -lc '
    #CFLAGS = DEVICE
    #CFLAGS += ' -save-temps=obj'
    #AFLAGS = '-c'+ DEVICE + ' -x assembler-with-cpp'
    #AFLAGS += ' -Iplatform -Ilibraries/n22/include -Ilibraries/n22/env_Eclipse'
    #AFLAGS += ' -Iplatform -Ilibraries/inc'
    #LFLAGS = DEVICE
    #LFLAGS += ' -Wl,--gc-sections,-cref,-Map=' + MAP_FILE
    #LFLAGS += ' -T ' + LINK_FILE
    #LFLAGS += ' -Wl,-wrap=memset'
    
    DEVICE = ' -march=rv32imfc '
    #DEVICE = ' -march=rv32imc '
    CFLAGS = '-g -Os -Wall '+ DEVICE
    #CFLAGS += ' -ffunction-sections -fdata-sections '
    CFLAGS += ' -ffunction-sections '
    CFLAGS += ' -Iplatform -Ilibraries/inc '
    AFLAGS = '-c -g '+ DEVICE
    AFLAGS += ' -Iplatform -Ilibraries/inc '
    LFLAGS = ' -nostartfiles -Wl,--gc-sections '
    LFLAGS += ' -T ' + LINK_FILE
    LFLAGS += ' -Wl,-Map=' + MAP_FILE

    CPATH = ''
    LPATH = ''

    #if BUILD == 'debug':
    #    CFLAGS += ' -O0 -g3'
    #    AFLAGS += ' -g3'
    #else:
    #    CFLAGS += ' -O2'

    #POST_ACTION = OBJCPY + ' -O binary $TARGET ' + TARGET_NAME + '\n'
    POST_ACTION = SIZE + ' $TARGET\n'
    POST_ACTION += 'bintools -d -u $TARGET ' + TARGET_NAME + '\n'
    # POST_ACTION += 'elf2bin.exe $TARGET\n'
    #POST_ACTION += 'elf2bin.exe -s 19000 $TARGET\n'
    # POST_ACTION += 'cp.exe -f flat.bin ' + TARGET_NAME + '\n'
    # POST_ACTION += 'rm.exe -f flat.bin \n'
    POST_ACTION += 'mv -f flat.bin ' + TARGET_NAME + '\n'

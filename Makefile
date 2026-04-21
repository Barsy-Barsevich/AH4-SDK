include config.txt

help:
	@echo "Following commands are expected:"
	@echo "- make build-libs [OPTIMIZATION_LEVEL=]"
	@echo "- make clear-libs"
	@echo "- make build-project target=<your-project-directory> [OPTIMIZATION_LEVEL=]"
	@echo "- make clear-project target=<your-project-directory>"
	@echo "- make upload firmware=*.hex"
	 


build-sdk:
	cd ./wch-isp
	make
	cd ..


FLASHER = sudo ./wch-isp/wch-isp
FIRMWARE_STARTADDR = 0x08000000

upload-via-bootloader:
	make -C wch-isp upload firmware=''../${firmware}''

upload-swd:
	./minichlink-307/minichlink-307 -w ${firmware} 0x08000000


PROJECT_DIR = ${target}

CC = ${TOOLCHAIN_PREFIX}-gcc
LD = ${TOOLCHAIN_PREFIX}-ld
NM = ${TOOLCHAIN_PREFIX}-nm
OBJDUMP = ${TOOLCHAIN_PREFIX}-objdump
OBJCOPY = ${TOOLCHAIN_PREFIX}-objcopy
SIZE = ${TOOLCHAIN_PREFIX}-size

BUILD_FLAGS = 
WFLAGS = \
	-pedantic-errors \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Wduplicated-branches \
	-Wduplicated-cond \
	-Wfloat-equal \
	-Wlogical-op \
	-Wsign-conversion \
	-Wrestrict \

FFLAGS = \
	-ffunction-sections \

ARCHFLAGS = 
ifneq (${ARCH},'')
	ARCHFLAGS += -march=${ARCH}
endif
ifneq (${ABI},'')
	ARCHFLAGS += -mabi=${ABI}
endif
ifneq (${CODE_MODEL},'')
	ARCHFLAGS += -mcmodel=${CODE_MODEL}
endif
BUILD_FLAGS = $(ARCHFLAGS) -${OPTIMIZATION_LEVEL} $(WFLAGS) $(FFLAGS)
BUILD_FLAGS += ${EXTRA_BUILD_FLAGS}
LINKER_FLAGS = --gc-sections
LINKER_FLAGS += ${EXTRA_LINKER_FLAGS}

INCLUDE_DIRS = \
	-I Core/MRS-Peripheral/inc \
	-I Core/MRS-Core \
	-I Core/MRS-FATFS/inc \
	-I Core/Devices/inc \
	-I Core/USB/inc
INCLUDE_DIRS += -I components/ICM45686_Barsotion/inc
# add project include dirs if config.txt file exists
PRJ_SRC_DIRS = 
ifneq ($(wildcard ${PROJECT_DIR}/config.txt),)
    include ${PROJECT_DIR}/config.txt
    INCLUDE_DIRS += $(addprefix -I${PROJECT_DIR}/, ${PROJECT_INCLUDE_DIRS})
	PRJ_SRC_DIRS += $(addprefix ${PROJECT_DIR}/, ${PROJECT_SOURCES_DIRS})
endif

COMPONENTS ?= \
	BMP3XX_Barsotion \
	ICM45686_Barsotion \

.PHONY: build-libs
build-libs: clear-libs build-core build-components

.PHONY: build-components
build-components: ${COMPONENTS}

.PHONY: ${COMPONENTS}
${COMPONENTS}:
	cd components/$@ && make PREFIX="${TOOLCHAIN_PREFIX}" FLAGS="${BUILD_FLAGS}"

.PHONY: build-core
build-core: clear-libs 
	@echo "=====<Compiling startup file>===================="
	${CC} ${BUILD_FLAGS} -c Core/startup.S -o Core/startup.o
	@echo "=====<Compiling MRS core libs>==================="
	mkdir Core/MRS-Core/build
	@for source in Core/MRS-Core/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		echo "${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/MRS-Core/build/$${OUT_FILENAME}.o"; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/MRS-Core/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling MRS peripheral libs>============="
	mkdir Core/MRS-Peripheral/build
	@for source in Core/MRS-Peripheral/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		echo "${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/MRS-Peripheral/build/$${OUT_FILENAME}.o"; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/MRS-Peripheral/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling MRS SDIO FATFS lib>=============="
	mkdir Core/MRS-FATFS/build
	@for source in Core/MRS-FATFS/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		echo "${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/MRS-FATFS/build/$${OUT_FILENAME}.o"; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/MRS-FATFS/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling Devices libs>===================="
	mkdir Core/Devices/build
	@for source in Core/Devices/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		echo "${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/Devices/build/$${OUT_FILENAME}.o"; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/Devices/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling USB module>======================"
	mkdir Core/USB/build
	@for source in Core/USB/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		echo "${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/USB/build/$${OUT_FILENAME}.o"; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/USB/build/$${OUT_FILENAME}.o; \
	done	
	@echo "=====<Making an archive>========================="
	${AR} rcs Core/libah4-sdk.a Core/MRS-Peripheral/build/* Core/MRS-Core/build/* Core/Devices/build/* \
		Core/USB/build/* Core/MRS-FATFS/build/* Core/startup.o
#	${AR} rcs Core/libah4-sdk.a Core/MRS-Peripheral/build/* Core/MRS-Core/build/* Core/Devices/build/* \
		Core/MRS-FATFS/build/* Core/startup.o
	@echo "=====<Totals>===================================="
	${SIZE} -t --format=berkeley Core/libah4-sdk.a

.PHONY: clear-libs
clear-libs:
	rm -rf Core/MRS-Core/build
	rm -rf Core/MRS-Peripheral/build
	rm -rf Core/MRS-FATFS/build
	rm -rf Core/Devices/build
	rm -rf Core/USB/build
	rm -f Core/*.a
	rm -rf components/*/build
	rm -f components/*/*.a

.PHONY: build-project
build-project: clear-project
	@echo "=====<Compiling project>========================="
	mkdir ${PROJECT_DIR}/build
	@for source in ${PROJECT_DIR}/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		echo "${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o ${PROJECT_DIR}/build/$${OUT_FILENAME}.o"; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o ${PROJECT_DIR}/build/$${OUT_FILENAME}.o; \
	done
	@if [ -f {PROJECT_DIR}/*.S ]; then \
		for source in ${PROJECT_DIR}/*.S; do \
			OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
			${CC} ${BUILD_FLAGS} -c $$source -o ${PROJECT_DIR}/build/$${OUT_FILENAME}.o; \
		done \
	fi
	@echo "=====<Linking everything together>==============="
	${CC} \
		$(ARCHFLAGS) \
		-Wl,--gc-sections,-Map,${PROJECT_DIR}/firmware.map \
		-T Core/linker.ld \
		${PROJECT_DIR}/build/*.o \
		Core/*.a \
		-lc_nano -lm_nano -lgloss -nostartfiles \
		components/*/*.a \
		-o ${PROJECT_DIR}/firmware.elf
	${OBJCOPY} -O ihex ${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/firmware.hex
	${OBJCOPY} -O binary ${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/firmware.bin
	${SIZE} -t --format=berkeley ${PROJECT_DIR}/firmware.elf

.PHONY: disasm-project
disasm-project:
	${OBJDUMP} -S ${PROJECT_DIR}/firmware.elf > ${PROJECT_DIR}/firmware.lst

.PHONY: clear-project
clear-project:
	rm -rf ${PROJECT_DIR}/build
	rm -f ${PROJECT_DIR}/*.elf
	rm -f ${PROJECT_DIR}/*.hex
	rm -f ${PROJECT_DIR}/*.bin
	rm -f ${PROJECT_DIR}/*.map
	rm -f ${PROJECT_DIR}/*.lst


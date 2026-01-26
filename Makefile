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

BUILD_FLAGS = -pedantic-errors \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Wduplicated-branches \
	-Wduplicated-cond \
	-Wfloat-equal \
	-Wlogical-op \
	-Wsign-conversion \
	-Wrestrict \
	-ffunction-sections
ifneq (${ARCH},'')
	BUILD_FLAGS += -march=${ARCH}
endif
ifneq (${ABI},'')
	BUILD_FLAGS += -mabi=${ABI}
endif
ifneq (${CODE_MODEL},'')
	BUILD_FLAGS += -mcmodel=${CODE_MODEL}
endif
BUILD_FLAGS += -${OPTIMIZATION_LEVEL}
BUILD_FLAGS += ${EXTRA_BUILD_FLAGS}
LINKER_FLAGS = --gc-sections
ifneq (${STDLIB_PATH},)
	LINKER_FLAGS += -L ${STDLIB_PATH}
endif
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

.PHONY: build-libs
build-libs:
	@echo "=====<Compiling startup file>===================="
	${CC} ${BUILD_FLAGS} -c Core/startup.S -o Core/startup.o
	@echo "=====<Compiling MRS core libs>==================="
	if [ -d "Core/MRS-Core/build" ]; then \
		rm -rf Core/MRS-Core/build; \
	fi
	mkdir Core/MRS-Core/build
	for source in Core/MRS-Core/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/MRS-Core/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling MRS peripheral libs>============="
	if [ -d "Core/MRS-Peripheral/build" ]; then \
		rm -rf Core/MRS-Peripheral/build; \
	fi
	mkdir Core/MRS-Peripheral/build
	for source in Core/MRS-Peripheral/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/MRS-Peripheral/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling MRS SDIO FATFS lib>=============="
	if [ -d "Core/MRS-FATFS/build" ]; then \
		rm -rf Core/MRS-FATFS/build; \
	fi
	mkdir Core/MRS-FATFS/build
	for source in Core/MRS-FATFS/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/MRS-FATFS/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling Devices libs>===================="
	if [ -d "Core/Devices/build" ]; then \
		rm -rf Core/Devices/build; \
	fi
	mkdir Core/Devices/build
	for source in Core/Devices/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/Devices/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling USB module>======================"
	if [ -d "Core/USB/build" ]; then \
		rm -rf Core/USB/build; \
	fi
	mkdir Core/USB/build
	for source in Core/USB/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/USB/build/$${OUT_FILENAME}.o; \
	done	
	@echo "=====<Making an archive>========================="
	if [ -f "Core/libah4-sdk.a" ]; then \
		rm Core/libah4-sdk.a; \
	fi
	${AR} rcs Core/libah4-sdk.a Core/MRS-Peripheral/build/* Core/MRS-Core/build/* Core/Devices/build/* \
		Core/USB/build/* Core/MRS-FATFS/build/* Core/startup.o
#	${AR} rcs Core/libah4-sdk.a Core/MRS-Peripheral/build/* Core/MRS-Core/build/* Core/Devices/build/* \
		Core/MRS-FATFS/build/* Core/startup.o
	@echo "=====<Totals>===================================="
	${SIZE} -t --format=berkeley Core/libah4-sdk.a
	@echo "=====<Building components>======================="
	if [ -d "components/ICM45686_Barsotion/build" ]; then \
		rm -rf components/ICM45686_Barsotion/build; \
	fi
	mkdir components/ICM45686_Barsotion/build
	@for component in components/*; do \
		make -C $$component build PREFIX=${TOOLCHAIN_PREFIX} FLAGS=" ${BUILD_FLAGS} "; \
	done

.PHONY: clear-libs
clear-libs:
	rm -r Core/MRS-Core/build
	rm -r Core/MRS-Peripheral/build
	rm -r Core/Devices/build
	rm -r Core/USB/build
	rm Core/*.o Core/*.a
	rm -r components/*/build
	rm components/*/*.a

build-project:
	@echo "=====<Compiling project>========================="
	if [ -d "${PROJECT_DIR}/build" ]; then \
		rm -rf ${PROJECT_DIR}/build; \
	fi
	mkdir ${PROJECT_DIR}/build
	for source in ${PROJECT_DIR}/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o ${PROJECT_DIR}/build/$${OUT_FILENAME}.o; \
	done
	@if [ -f {PROJECT_DIR}/*.S ]; then \
		for source in ${PROJECT_DIR}/*.S; do \
			OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
			${CC} ${BUILD_FLAGS} -c $$source -o ${PROJECT_DIR}/build/$${OUT_FILENAME}.o; \
		done \
	fi
	@echo "=====<Linking everything together>==============="
	${LD} -T Core/linker.ld ${LINKER_FLAGS} --format=elf32-littleriscv --output=${PROJECT_DIR}/firmware.elf -Map ${PROJECT_DIR}/firmware.map ${PROJECT_DIR}/build/*.o Core/*.a -lc -lgloss components/*/*.a
	${OBJCOPY} -O ihex ${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/firmware.hex
	${OBJCOPY} -O binary ${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/firmware.bin
	${SIZE} -t --format=berkeley ${PROJECT_DIR}/firmware.elf

disasm-project:
	${OBJDUMP} -S ${PROJECT_DIR}/firmware.elf > ${PROJECT_DIR}/firmware.lst

clear-project:
	rm -r ${PROJECT_DIR}/build
	rm ${PROJECT_DIR}/*.elf ${PROJECT_DIR}/*.hex

	

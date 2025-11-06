
help:
	@echo "* make upload firmware=*.hex" 


build-sdk:
	cd ./wch-isp
	make
	cd ..


FLASHER = sudo ./wch-isp/wch-isp
FIRMWARE_STARTADDR = 0x08000000

upload:
	${FLASHER} --address=${FIRMWARE_STARTADDR} -p -r write ${firmware}



PROJECT_DIR = ${target}

PREFIX = riscv64-unknown-elf
CC = ${PREFIX}-gcc
LD = ${PREFIX}-ld
NM = ${PREFIX}-nm
OBJDUMP = ${PREFIX}-objdump
OBJCOPY = ${PREFIX}-objcopy
SIZE = ${PREFIX}-size

ARCH = rv32imafc
ABI = ilp32f
BUILD_FLAGS = ''-march=${ARCH} -mabi=${ABI} -O2''
INCLUDE_DIRS = ''-I Core/Peripheral/inc -I Core/Core''

.PHONY: build-project
build-project:
	echo "${CC} ${BUILD_FLAGS} -c Core/startup.S -o startup.o"
	mkdir ${PROJECT_DIR}/build || echo "build dir already created."
	for source in ${PROJECT_DIR}/*.c; do \
		echo $$source;\
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o $$source.o; \
	done
	${CC} ${BUILD_FLAGS} -c Core/startup.S -o Core/startup.o
	for source in Core/Core/*.c; do \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o $$source.o; \
	done
	for source in Core/Peripheral/src/*.c; do \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o $$source.o; \
	done
	${LD} -T Core/linker.ld --format=elf32-littleriscv --output=${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/*.o Core/startup.o Core/Core/*.o Core/Peripheral/src/*.o
	rm ${PROJECT_DIR}/*.o
	${OBJCOPY} -O ihex ${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/firmware.hex
	${SIZE} -t --format=berkeley ${PROJECT_DIR}/firmware.elf


build-clear:
	rm Core/Core/*.o
	rm Core/Peripheral/src/*.o
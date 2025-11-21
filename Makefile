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

upload:
	make -C wch-isp upload firmware=''../${firmware}''



PROJECT_DIR = ${target}

CC = ${TOOLCHAIN_PREFIX}-gcc
LD = ${TOOLCHAIN_PREFIX}-ld
NM = ${TOOLCHAIN_PREFIX}-nm
OBJDUMP = ${TOOLCHAIN_PREFIX}-objdump
OBJCOPY = ${TOOLCHAIN_PREFIX}-objcopy
SIZE = ${TOOLCHAIN_PREFIX}-size

BUILD_FLAGS = 
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

INCLUDE_DIRS = -I Core/Peripheral/inc -I Core/Core -I Core/Devices/inc -I Core/USB/inc

.PHONY: build-libs
build-libs:
	@echo "=====<Compiling startup file>===================="
	${CC} ${BUILD_FLAGS} -c Core/startup.S -o Core/startup.o
	@echo "=====<Compiling MRS core libs>==================="
	mkdir Core/Core/build || echo "build dir already created."
	for source in Core/Core/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/Core/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling MRS peripheral libs>============="
	mkdir Core/Peripheral/build || echo "build dir already created."
	for source in Core/Peripheral/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/Peripheral/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling Devices libs>===================="
	mkdir Core/Devices/build || echo "build dir already created."
	for source in Core/Devices/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/Devices/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Compiling USB module>======================"
	mkdir Core/USB/build || echo "build dir already created."
	for source in Core/USB/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/USB/build/$${OUT_FILENAME}.o; \
	done	
	@echo "=====<Making an archive>========================="
	${AR} rcs Core/libch32v30x.a Core/Peripheral/build/* Core/Core/build/* Core/Devices/build/* \
		Core/USB/build/* Core/startup.o
	@echo "=====<Totals>===================================="
	${SIZE} -t --format=berkeley Core/libch32v30x.a
	@echo "=====<Building components>======================="
	@for component in components/*; do \
		make -C $$component build-debug; \
	done

.PHONY: clear-libs
clear-libs:
	rm -r Core/Core/build
	rm -r Core/Peripheral/build
	rm -r Core/Devices/build
	rm -r Core/USB/build
	rm Core/*.o Core/*.a
	rm components/*/*.a

build-project:
	@echo "=====<Compiling project>========================="
	mkdir ${PROJECT_DIR}/build || echo "build dir already created."
	for source in ${PROJECT_DIR}/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o ${PROJECT_DIR}/build/$${OUT_FILENAME}.o; \
	done
	@echo "=====<Linking everything together>==============="
	${LD} -T Core/linker.ld --format=elf32-littleriscv --output=${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/build/*.o Core/*.a -L /opt/ex-riscv64-unknown-elf/riscv64-unknown-elf/lib/rv32imafc_zicsr_zaamo_zalrsc/ilp32f -lc
	#components/*/*.a
	${OBJCOPY} -O ihex ${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/firmware.hex
	${SIZE} -t --format=berkeley ${PROJECT_DIR}/firmware.elf


clear-project:
	rm -r ${PROJECT_DIR}/build
	rm ${PROJECT_DIR}/*.elf ${PROJECT_DIR}/*.hex

	

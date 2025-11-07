include config.txt

help:
	@echo "Following commands are expected:"
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
BUILD_FLAGS += -${OPTIMIZATION_LEVEL}

INCLUDE_DIRS = -I Core/Peripheral/inc -I Core/Core


build-project:
	
	mkdir ${PROJECT_DIR}/build || echo "build dir already created."
	for source in ${PROJECT_DIR}/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o ${PROJECT_DIR}/build/$${OUT_FILENAME}.o; \
	done
	
	${CC} ${BUILD_FLAGS} -c Core/startup.S -o Core/startup.o
	
	mkdir Core/Core/build || echo "build dir already created."
	for source in Core/Core/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/Core/build/$${OUT_FILENAME}.o; \
	done
	
	mkdir Core/Peripheral/build || echo "build dir already created."
	for source in Core/Peripheral/src/*.c; do \
		OUT_FILENAME=`echo $$source | awk -F'/' '{print $$NF}'`; \
		${CC} ${BUILD_FLAGS} ${INCLUDE_DIRS} -c $$source -o Core/Peripheral/build/$${OUT_FILENAME}.o; \
	done
	
	${AR} rcs Core/libch32v30x.a Core/Peripheral/build/* Core/Core/build/* Core/startup.o
	
	${LD} -T Core/linker.ld --format=elf32-littleriscv --output=${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/build/*.o Core/*.a
	
	${OBJCOPY} -O ihex ${PROJECT_DIR}/firmware.elf ${PROJECT_DIR}/firmware.hex
	${SIZE} -t --format=berkeley ${PROJECT_DIR}/firmware.elf


clear-project:
	rm -r ${PROJECT_DIR}/build
	rm ${PROJECT_DIR}/.elf ${PROJECT_DIR}/.hex


build-clear:
	rm Core/Core/*.o
	rm Core/Peripheral/src/*.o
	

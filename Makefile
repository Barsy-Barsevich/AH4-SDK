
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



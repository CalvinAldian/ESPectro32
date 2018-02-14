deps_config := \
	/home/asus/esp-idf/components/app_trace/Kconfig \
	/home/asus/esp-idf/components/aws_iot/Kconfig \
	/home/asus/esp-idf/components/bt/Kconfig \
	/home/asus/esp-idf/components/esp32/Kconfig \
	/home/asus/esp-idf/components/ethernet/Kconfig \
	/home/asus/esp-idf/components/fatfs/Kconfig \
	/home/asus/esp-idf/components/freertos/Kconfig \
	/home/asus/esp-idf/components/heap/Kconfig \
	/home/asus/esp-idf/components/libsodium/Kconfig \
	/home/asus/esp-idf/components/log/Kconfig \
	/home/asus/esp-idf/components/lwip/Kconfig \
	/home/asus/esp-idf/components/mbedtls/Kconfig \
	/home/asus/esp-idf/components/openssl/Kconfig \
	/home/asus/esp-idf/components/pthread/Kconfig \
	/home/asus/esp-idf/components/spi_flash/Kconfig \
	/home/asus/esp-idf/components/spiffs/Kconfig \
	/home/asus/esp-idf/components/tcpip_adapter/Kconfig \
	/home/asus/esp-idf/components/wear_levelling/Kconfig \
	/home/asus/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/asus/esp-idf/components/esptool_py/Kconfig.projbuild \
	/c/esp/blink/main/Kconfig.projbuild \
	/home/asus/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/asus/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;

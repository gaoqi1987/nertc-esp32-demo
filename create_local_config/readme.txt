example:
python $IDF_PATH/components/spiffs/spiffsgen.py 0x1000 local_config config.bin
esptool.py --chip esp32-s3 write_flash 0x576000 config.bin

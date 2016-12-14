cmd /k "avrdude -c usbasp-clone -p m168 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xf9:m"


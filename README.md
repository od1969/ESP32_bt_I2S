# ESP32_bt_I2S
bluetooth receiver using I2S

* 外付けDAC(PCM5102A)から音声を出力します。
* Espressif toolchain(20170330版),ESP-IDF Release v2.1

## 接続

ESP32-DevKitC　→　PHAT DAC 

5V　→　5V 

GND　→　GND 

IO26 →　BITCLOCK(GPIO18) 

IO25 →　LRCLOCL(GPIO19) 

IO27 →　DATAIN(GPIO20) 

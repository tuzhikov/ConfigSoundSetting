#ifndef CMD_DEVICE_H
#define CMD_DEVICE_H

#include <stdint.h>
#include <time.h>

enum TYPE_TRACK{ZSP, ZSR, ZSVRR, ZSO, ZSZ, ZSVRZ};

enum TYPE_ERROR{
    ERR_FLH     = 0x00000001, // ошибка записи flash
    ERR_DP      = 0x00000002, // ошибка цифрового потенциометра
    ERR_ETH     = 0x00000004, //  ошибка работы esp8266
    ERR_GPS     = 0x00000008  //  ошибка работы gps
};

enum FILE_HEAD_WAV{
    MAX_LENGH_FILE_NAME     = 64,
    MAX_LENGH_HEAD_WAV    = 44,
    END_FILE_PREFIX
};

const uint8_t HeaderProtocol [] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0xC3, 0x5A };

enum HEADER_PROTOCOL{
    MAX_LENGH_HEADER = sizeof(HeaderProtocol),
    END_HEADER_PROTOCOL
};


#pragma pack(push,1)
// data error
struct DATAERROR {
    uint32_t error;
    uint16_t ver; // версия прошивки.
};
// structure data test track
struct TYPE_TEST_TRACK {
    uint16_t value_general; // общий коэфф. громкости
    uint16_t value_sk1;     // уровень громкости на динамике 1
    uint16_t value_sk2;     // уровень громкости на динамике 2
    uint16_t value_plan;    // уровень громкости по плану
    uint16_t noise;         // коэффициент датчика шума.
    uint8_t number_music;   // номер мелодии ЗСП, ЗСР, ЗСВРР, ЗСО, ЗСЗ, ЗСВРЗ
};
// structure time RTC
struct TYPETIME{
    uint64_t time;  // представление времени с сек.
    uint8_t gmt;    // часовой пояс
};
// structure test
struct TYPE_TEST {
    uint64_t time;                  //
    uint64_t flag;                  //
    uint8_t plan_number;            //
    uint8_t plan_item;              //
    uint8_t plan_value;             //
    uint8_t value_speaker1;         //
    uint8_t value_speaker2;         //
    uint8_t noise_level;            //
    uint8_t noise_sensor_volume;    //
};
// START WAV
struct TYPE_WAV_PREFIX {
    uint8_t file_name [MAX_LENGH_FILE_NAME];
    uint8_t head_wav [MAX_LENGH_HEAD_WAV];
    uint16_t packet_number;
    uint8_t number_group;
};

#pragma pack(pop)

#endif // CMD_DEVICE_H

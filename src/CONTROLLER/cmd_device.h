#ifndef CMD_DEVICE_H
#define CMD_DEVICE_H

#include <stdint.h>
#include <time.h>

#define COD_CLEAR_WAV 32

enum TYPE_TRACK{
    btZSR, btZSO,btZSP, btZSZ,btZSVRR, btZSVRZ};

enum TYPE_NUMBER_COMMAND_TRANSPORT{
    CMD_TR_WRITE = 0x01,
    CMD_TR_READ  = 0x02,
    END_TYPE_NUMBER_COMMAND_TRANSPORT
};

enum TYPE_NUMBER_COMMAND{
    CMD_TEST    = 0x00,
    CMD_PRJ    = 0x01,
    CMD_WAV    = 0x02,
    CMD_TIME    = 0x03,
    END_TYPE_NUMBER_COMMAND
};

enum TYPE_NUMBER_COMMAND_TEST{
    CMD_TEST_LISTEN     = 0x00,
    CMD_TEST_DEVICE     = 0x01,
    CMD_TEST_ERROR      = 0xEE,
    END_TYPE_NUMBER_COMMAND_TEST
};

enum TYPE_DEVICE{
    TYPE_UZTVOP = 0x10,
    END_TYPE_DEVICE
};

enum TYPE_ERROR{
    ERR_FLH     = 0x00000001, // ошибка записи flash
    ERR_DP      = 0x00000002, // ошибка цифрового потенциометра
    ERR_ETH     = 0x00000004, //  ошибка работы esp8266
    ERR_GPS     = 0x00000008  //  ошибка работы gps
};

enum TYPE_TEST_FLAG{
    TEST_RED        = 0x00000001, // активный красный
    TEST_GREEN      = 0x00000002, // активный зеленый
    TEST_WAIT       = 0x00000004, // активный ждите
    TEST_GPS        = 0x00000008, // активный GPS (отвечает есть один спутик)
    TEST_BUTTON     = 0x00000010, // кнопка нажата
    TEST_MOTION     = 0x00000020, // датчик движения сработал
    END_TYPE_TEST_FLAG
};

enum FILE_HEAD_WAV{
    MAX_LENGH_FILE_NAME = 64,
    MAX_LENGH_HEAD_WAV  = 44,
    END_FILE_PREFIX
};

const uint8_t HeaderProtocol1 [] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
const uint8_t HeaderProtocol2 [] = { 0x55, 0xC3, 0x5A };
//
struct HEADER{
    char *pHeader1;
    char *pHeader2;
    uint32_t lengn1;
    uint32_t lengn2;
};
const HEADER header = {
    .pHeader1 = (char*)HeaderProtocol1,
    .pHeader2 = (char*)HeaderProtocol2,
    .lengn1 = sizeof(HeaderProtocol1),
    .lengn2 = sizeof(HeaderProtocol2)
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
    uint64_t time;                  // время прибора
    uint64_t flagTest;              // сигналы и нажатые ктопки
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

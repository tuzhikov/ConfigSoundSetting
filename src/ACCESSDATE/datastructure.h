#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <stdint.h>

enum MAX_PROJRCT{
    MAX_SPEAKER         = 2,    // динамика
    MAX_SPEAKER_LEVEL   = 7,    // уровней громкости для 5 мелодий
    MAX_DAY_WEEK        = 7,    //
    MAX_PLAN_ITEMS      = 24,   // планов в день
    MAX_DAY_PLANS       = 24,   // дней с заданными планами
    MAX_HOLIDAYS        = 120,  // праздничный дней
    END_ENUM
};

#pragma pack(push,1)
// описание одного плана
struct TYPEPLANITEMS
{
    uint8_t hour;
    uint8_t min;
    uint8_t value_speaker1; //%
    uint8_t value_speaker2; //%
};
// количество планов в одном дне
struct TYPEDAY
{
    TYPEPLANITEMS Items[MAX_PLAN_ITEMS];
};
// план на один день недели
struct TYPEWEEK
{
    uint8_t plan;
    uint8_t reserved;
};
// план для праздничного дня.
struct TYPEHOLIDAY
{
    uint8_t month;
    uint8_t day;
    uint8_t plan;
    uint8_t reserved;
};
// уровни громкости одного динамика
struct TYPESOUNDVALUE
{
    uint8_t global; // коэффициент от громкости %
    uint8_t separate[MAX_SPEAKER_LEVEL];
};
// уровни громкости всех динамиков
struct TYPEALLSOUND
{
    TYPESOUNDVALUE  value_sound[MAX_SPEAKER];
};
// настройка датчика шума
struct TYPENOISE
{
    uint32_t time;
    uint8_t min_level;
    uint8_t max_level;
    uint8_t min_ct;
    uint8_t max_ct;
    uint8_t on;
    uint8_t reserved;
};
//датчик движения
struct TYPEMOTION
{
    uint32_t time;
    uint8_t asTVP;
    uint8_t on;
    uint8_t reserved;
};
// Sound acceleration
struct TYPEACCELERATION
{
    uint32_t time;
    uint8_t on;
};
// настройка кнопки
struct TYPEBUTTON
{
    uint32_t time;
    uint8_t on;
};
// вибрация
struct TYPEVIBRATION
{
    uint32_t    time_lenght;
    uint32_t    time_pause;
    uint32_t    intensity;
    uint8_t     on;
};
// настройка флагов
struct TYPESETTING
{
    uint64_t flag;
    uint32_t period;
    uint32_t delay;
};
// project data
struct TYPEPROJECT{
    TYPEHOLIDAY         holidays[MAX_HOLIDAYS];     //праздничные дни
    TYPEDAY             day_plans[MAX_DAY_PLANS];   //дни с планами
    TYPEWEEK            week[MAX_DAY_WEEK];         //неделя с планами
    TYPEALLSOUND        sound_value;                //настройка громкости
    TYPENOISE           noise;                      //датчик шума
    TYPEMOTION          motion;                     //датчик движения
    TYPEACCELERATION    acceleration;               //ускорение звука.
    TYPEBUTTON          button;                     //кнопка
    TYPEVIBRATION       vibration;                  //двигатель вибрации
    TYPESETTING         setting;                    //общие настройки.
};
#pragma pack(pop)

#endif // DATASTRUCTURE_H

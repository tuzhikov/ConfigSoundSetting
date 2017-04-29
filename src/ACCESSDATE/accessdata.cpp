#include <cstring>
#include "accessdata.h"

using namespace ACCESS_DATE;


AccessData::AccessData():pDataProject(new TYPEPROJECT)
{
    // default configure
    setDefault(pDataProject);
}

AccessData::~AccessData()
{
    delete pDataProject;
}
/**
*/
/*AccessData::QByteArray getAllDate() const
{
    QByteArray tmp((char)&data,sizeof(data));
    return tmp;
}*/
void AccessData::setDefault(TYPEPROJECT *const pPrg)
{
    pPrg->acceleration.on = true;
    pPrg->acceleration.time = 100;

    pPrg->button.on = true;
    pPrg->button.time = 100;

    pPrg->vibration.on = true;
    pPrg->vibration.intensity = 1000;
    pPrg->vibration.time_lenght = 10;
    pPrg->vibration.time_pause = 10;

    pPrg->motion.on = false;

    pPrg->noise.on = false;
}
/**
*/
void AccessData::setSetting (   const uint64_t flag,
                                const uint32_t period,
                                const uint32_t delay )

{
    pDataProject->setting.flag = flag;
    pDataProject->setting.period = period;
    pDataProject->setting.delay = delay;
}
/**
*/
void AccessData::setSetting ( const TYPESETTING prm )
{
    TYPESETTING *const pSett = &pDataProject->setting;
    memcpy(pSett,&prm,sizeof(TYPESETTING));
}
/**
*/
void AccessData::setVibration(  const uint32_t time_lenght,
                                const uint32_t time_pause,
                                const uint32_t intensity,
                                const uint8_t on)
{
    pDataProject->vibration.time_lenght = time_lenght;
    pDataProject->vibration.time_pause = time_pause;
    pDataProject->vibration.intensity = intensity;
    pDataProject->vibration.on = on;

}
/**
*/
void AccessData::setVibration( const TYPEVIBRATION prm )
{
    TYPEVIBRATION *const pVib = &pDataProject->vibration;
    memcpy(pVib,&prm,sizeof(TYPEVIBRATION));
}
/**
*/
void AccessData::setButton( const uint32_t time,
                            const uint8_t on)
{
    pDataProject->button.time = time;
    pDataProject->button.on = on;
}
/**
*/
void AccessData::setButton( const TYPEBUTTON prm )
{
    TYPEBUTTON *const pPrm = &pDataProject->button;
    memcpy(pPrm,&prm,sizeof(TYPEBUTTON));
}
/**
*/
void AccessData::setAcceleration( const uint32_t time,
                                  const uint8_t on)
{
    pDataProject->acceleration.time = time;
    pDataProject->acceleration.on = on;
}
/**
*/
void AccessData::setAcceleration( const TYPEACCELERATION prm )
{
    TYPEACCELERATION *const pPrm = &pDataProject->acceleration;
    memcpy(pPrm,&prm,sizeof(TYPEACCELERATION));
}
/**
*/
void AccessData::setMotion( const uint8_t on )
{
    pDataProject->motion.on = on;
}
/**
*/
void AccessData::setMotion( const TYPEMOTION prm )
{
    TYPEMOTION *const pPrm = &pDataProject->motion;
    memcpy(pPrm,&prm,sizeof(TYPEMOTION));
}
/**
*/
bool AccessData::setOnePlanDay( const uint8_t num_plan,
                                const uint8_t item,
                                const uint8_t hour,
                                const uint8_t min,
                                const uint8_t value_speaker1,
                                const uint8_t value_speaker2 )
{
    if ( (num_plan<getMaxPlans())&&(item<getMaxItemDday()) )
    {
        pDataProject->day_plans[num_plan].Items[item].hour = hour;
        pDataProject->day_plans[num_plan].Items[item].min = min;
        pDataProject->day_plans[num_plan].Items[item].value_speaker1 = value_speaker1;
        pDataProject->day_plans[num_plan].Items[item].value_speaker2 = value_speaker2;
        return true;
    }
    return false;
}
/**
*/
bool AccessData::setOnePlanDay( const uint8_t num_plan,
                                const uint8_t item,
                                const TYPEPLANITEMS prm )
{
    if ( (num_plan<getMaxPlans())&&(item<getMaxItemDday()) )
    {
        TYPEPLANITEMS *const pPrm = &pDataProject->day_plans[num_plan].Items[item];
        memcpy(pPrm,&prm,sizeof(TYPEPLANITEMS));
        return true;
    }
    return false;
}
/**
*/
bool AccessData::setOneHoliday( const uint8_t item,
                                const uint8_t month,
                                const uint8_t day,
                                const uint8_t plan)
{
    if ( item<getMaxHoliday() )
    {
        pDataProject->holidays[item].month = month;
        pDataProject->holidays[item].day = day;
        pDataProject->holidays[item].plan = plan;
    }
    return false;
}
/**
*/
bool AccessData::setOneHoliday( const uint8_t item,
                                const TYPEHOLIDAY prm )
{
    if ( item<getMaxHoliday() )
    {
        TYPEHOLIDAY *const pPrm = &pDataProject->holidays[item];
        memcpy(pPrm,&prm,sizeof(TYPEHOLIDAY));
        return true;
    }
    return false;
}
/**
*/
void AccessData::setOneWeek( const uint8_t day, const uint8_t plan )
{
    pDataProject->week.day = day;
    pDataProject->week.plan = plan;
}
/**
*/
void AccessData::setOneWeek( const TYPEWEEK prm )
{
    TYPEWEEK *const pPrm = &pDataProject->week;
    memcpy(pPrm,&prm,sizeof(TYPEWEEK));
}
/**
*/
bool AccessData::setSoundValue (const uint8_t numspeaker,
                                const uint8_t global,
                                const uint8_t separate[],
                                const uint8_t size )
{
    if ( (numspeaker<getMaxSpeaker())&&(size<getMaxSpeakerLevel()) )
    {
        pDataProject->sound_value.value_sound[numspeaker].global = global;
        uint8_t *const pPrm = pDataProject->sound_value.value_sound[numspeaker].separate;
        memcpy(pPrm,separate,size);
        return true;
    }
    return false;
}
/**
*/
bool AccessData::setSoundValue (const uint8_t numspeaker, const TYPESOUNDVALUE prm)
{
    if ( numspeaker<getMaxSpeaker())
    {
        TYPESOUNDVALUE *const pPrm = &pDataProject->sound_value.value_sound[numspeaker];
        memcpy(pPrm,&prm,sizeof(TYPESOUNDVALUE));
        return true;
    }
    return false;
}
/**
*/
bool AccessData::getSetting (   uint64_t *const flag,
                                uint32_t *const period,
                                uint32_t *const delay )
{

    *flag = pDataProject->setting.flag;
    *period = pDataProject->setting.period;
    *delay = pDataProject->setting.delay;

    return true;
}
/**
*/
bool AccessData::getSetting ( TYPESETTING *const prm )
{
    prm->delay = pDataProject->setting.delay;
    prm->period = pDataProject->setting.period;
    prm->flag = pDataProject->setting.flag;
    return true;
}
/**
*/
bool AccessData::getVibration(  uint32_t *const time_lenght,
                                uint32_t *const time_pause,
                                uint32_t *const intensity,
                                uint8_t *const on)
{
    *time_lenght = pDataProject->vibration.time_lenght;
    *time_pause = pDataProject->vibration.time_pause;
    *intensity = pDataProject->vibration.intensity;
    *on = pDataProject->vibration.on;
    return true;
}
/**
*/
bool AccessData::getVibration( TYPEVIBRATION *const prm )
{
    prm->time_lenght = pDataProject->vibration.time_lenght;
    prm->time_pause = pDataProject->vibration.time_pause;
    prm->intensity = pDataProject->vibration.intensity;
    prm->on = pDataProject->vibration.on;
    return true;
}
/**
*/
bool AccessData::getButton( uint32_t *const time,
                            uint8_t *const on)
{
    *time = pDataProject->button.time;
    *on = pDataProject->button.on;
    return true;
}
/**
*/
bool AccessData::getButton( TYPEBUTTON *prm )
{
    prm->time = pDataProject->button.time;
    prm->on = pDataProject->button.on;
    return true;
}
/**
*/
bool AccessData::getAcceleration(   uint32_t *const time,
                                    uint8_t *const on)
{
    *time = pDataProject->acceleration.time;
    *on = pDataProject->acceleration.on;
    return true;
}
/**
*/
bool AccessData::getAcceleration( TYPEACCELERATION *const prm )
{
    prm->time = pDataProject->acceleration.time;
    prm->on = pDataProject->acceleration.on;
    return true;
}
/**
*/
bool AccessData::getMotion( uint8_t *const on )
{
    *on = pDataProject->motion.on;
    return true;
}
/**
*/
bool AccessData::getMotion( TYPEMOTION *const prm )
{
    prm->on = pDataProject->motion.on;
    return true;
}
/**
*/
bool AccessData::getOnePlanDay( const uint8_t num_plan,
                                const uint8_t item,
                                uint8_t *const hour,
                                uint8_t *const min,
                                uint8_t *const value_speaker1,
                                uint8_t *const value_speaker2 )
{
    if ( (num_plan<getMaxPlans())&&(item<getMaxItemDday()) )
    {
        *hour = pDataProject->day_plans[num_plan].Items[item].hour;
        *min = pDataProject->day_plans[num_plan].Items[item].min;
        *value_speaker1 = pDataProject->day_plans[num_plan].Items[item].value_speaker1;
        *value_speaker2 = pDataProject->day_plans[num_plan].Items[item].value_speaker2;
        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOnePlanDay( const uint8_t num_plan,
                                const uint8_t item,
                                TYPEPLANITEMS *const prm )
{
    if ( (num_plan<getMaxPlans())&&(item<getMaxItemDday()) )
    {
        prm->hour = pDataProject->day_plans[num_plan].Items[item].hour;
        prm->min = pDataProject->day_plans[num_plan].Items[item].min;
        prm->value_speaker1 = pDataProject->day_plans[num_plan].Items[item].value_speaker1;
        prm->value_speaker2 = pDataProject->day_plans[num_plan].Items[item].value_speaker2;
        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOneHoliday( const uint8_t item,
                                uint8_t *const month,
                                uint8_t *const day,
                                uint8_t *const plan)
{
    if (item<getMaxHoliday())
    {
        *month = pDataProject->holidays[item].month;
        *day = pDataProject->holidays[item].day;
        *plan = pDataProject->holidays[item].plan;
        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOneHoliday( const uint8_t item,
                                TYPEHOLIDAY *const prm )
{
    if (item<getMaxHoliday())
    {
        prm->month = pDataProject->holidays[item].month;
        prm->day = pDataProject->holidays[item].day;
        prm->plan = pDataProject->holidays[item].plan;
        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOneWeek( uint8_t *const day,
                             uint8_t *const plan )
{
    *day = pDataProject->week.day;
    *plan = pDataProject->week.plan;
    return true;
}
/**
*/
bool AccessData::getOneWeek( TYPEWEEK *const prm )
{
    prm->day = pDataProject->week.day;
    prm->plan = pDataProject->week.plan;
    return true;
}
/**
*/
bool AccessData::getSoundValue (const uint8_t numspeaker,
                                const uint8_t size,
                                uint8_t *const global,
                                uint8_t *const pMass)
{
    if ((numspeaker<getMaxSpeaker())&&(size<getMaxSpeakerLevel()))
    {
        uint8_t *const pStr = pDataProject->sound_value.value_sound[numspeaker].separate;
        memcpy(pMass,pStr,size);
        *global = pDataProject->sound_value.value_sound[numspeaker].global;
        return true;
    }
    return false;
}

bool AccessData::getSoundValue (const uint8_t numspeaker,
                                const uint8_t size,
                                TYPESOUNDVALUE *const prm)
{
    if ((numspeaker<getMaxSpeaker())&&(size<getMaxSpeakerLevel()))
    {
        uint8_t *const pStr = pDataProject->sound_value.value_sound[numspeaker].separate;
        memcpy(prm->separate,pStr,size);
        prm->global = pDataProject->sound_value.value_sound[numspeaker].global;
        return true;
    }
    return false;
}

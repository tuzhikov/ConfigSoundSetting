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
 * @brief AccessData::setDefault
 * @param pPrg
 */
void AccessData::setDefault(TYPEPROJECT *const pPrg)
{
    // clear
    clearProject();
    // set default data
    // volume
    for (int i=0;i<MAX_SPEAKER; i++)
    {
        pPrg->sound_value.value_sound[i].global = 100;
        for (int j=0; j<MAX_SPEAKER_LEVEL; j++)
        {
            pPrg->sound_value.value_sound[i].separate[j] = 50;
        }
    }
    //vibration
    pPrg->vibration.on = true;
    pPrg->vibration.intensity = 50;
    pPrg->vibration.time_lenght = 100;
    pPrg->vibration.time_pause = 100;
    //acceleration
    pPrg->acceleration.on = true;
    pPrg->acceleration.time = 4;
    //button
    pPrg->button.on = true;
    pPrg->button.time = 5;
    //motion
    pPrg->motion.on = false;
    pPrg->motion.asTVP = false;
    pPrg->motion.time = 5;
    //noice
    pPrg->noise.on = false;
    pPrg->noise.time = 1;
    pPrg->noise.max_level = 70;
    pPrg->noise.min_level = 30;
    pPrg->noise.max_ct = 10;
    pPrg->noise.min_ct = 10;
    // setting
    pPrg->setting.flag = GREEN_BLINK_TABU|ZSVRZ_PERIODICALLY|ZSVRR_PERIODICALLY;
    pPrg->setting.delay = 200;
    pPrg->setting.period = 200;
}
/**
 * @brief AccessData::clearProject
 */
void AccessData::clearProject()
{
    memset(pDataProject,0,sizeof(TYPEPROJECT));
    clearHoliday();
    clearPlans();
}
/**
 * @brief AccessData::setNoise
 * @param time
 * @param min_level
 * @param max_level
 * @param min_ct
 * @param max_ct
 * @param on
 * @return
 */
bool AccessData::setNoise(const uint32_t time,
                          const uint8_t min_level,
                          const uint8_t max_level,
                          const uint8_t min_ct,
                          const uint8_t max_ct,
                          const uint8_t on)
{
    pDataProject->noise.time = time;
    pDataProject->noise.min_level = min_level;
    pDataProject->noise.max_level = max_level;
    pDataProject->noise.min_ct = min_ct;
    pDataProject->noise.max_ct = max_ct;
    pDataProject->noise.on = on;
    return true;
}
/**
 * @brief AccessData::setNoise
 * @param prm
 * @return
 */
bool AccessData::setNoise(const TYPENOISE prm)
{
    TYPENOISE *const pNoise = &pDataProject->noise;
    memcpy(pNoise,&prm,sizeof(TYPENOISE));
    return true;
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
void AccessData:: setAcceleration( const TYPEACCELERATION prm )
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
 * @brief AccessData::clearHoliday
 * @return
 */
bool AccessData::clearHoliday()
{
    for ( uint32_t i=0; i<getMaxHoliday(); i++ )
    {
        TYPEHOLIDAY *const pPrm = &pDataProject->holidays[i];
        pPrm->month = 0xFF;
        pPrm->day = 0xFF;
        pPrm->plan = 1;
    }
    return true;
}
/**
 * @brief AccessData::clearPlans
 * @return
 */
bool AccessData::clearPlans()
{
    for ( uint32_t i=0; i<getMaxPlans(); i++ )
    {
        for ( uint32_t j=0; j<getMaxItemDday(); j++ )
        {
            TYPEPLANITEMS *const pday = &pDataProject->day_plans[i].Items[j];
            pday->hour = 0xFF;
            pday->min = 0xFF;
            pday->value_speaker1 = 0;
            pday->value_speaker2 = 0;
        }
    }
    return true;
}
/**
*/
bool AccessData::setOneWeek( const uint8_t day, const uint8_t plan )
{
    if ( day<MAX_DAY_WEEK )
    {
        pDataProject->week[day].plan = plan;
        return true;
    }
    return false;
}
/**
*/
bool AccessData::setOneWeek( const uint8_t day, const TYPEWEEK *prm )
{
    if ( day<MAX_DAY_WEEK )
    {
        TYPEWEEK *const pPrm = &pDataProject->week[day];
        memcpy(pPrm,prm,sizeof(TYPEWEEK));
        return true;
    }
    return false;
}
/**
*/
bool AccessData::setSoundValue (const uint8_t numspeaker,
                                const uint8_t global,
                                const uint8_t separate[],
                                const uint8_t size )
{
    if ( (numspeaker<getMaxSpeaker())&&(size<=getMaxSpeakerLevel()) )
    {
        pDataProject->sound_value.value_sound[numspeaker].global = global;
        uint8_t *const pPrm = pDataProject->sound_value.value_sound[numspeaker].separate;
        const size_t lengh = sizeof(pDataProject->sound_value.value_sound[numspeaker].separate);
        memcpy(pPrm,separate,lengh);
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
 * @brief AccessData::getNoise
 * @param time
 * @param min_level
 * @param max_level
 * @param min_ct
 * @param max_ct
 * @param on
 * @return
 */
bool AccessData::getNoise(uint32_t * const time,
                          uint8_t * const min_level,
                          uint8_t * const max_level,
                          uint8_t * const min_ct,
                          uint8_t * const max_ct,
                          uint8_t * const on)
{
    *time = pDataProject->noise.time;
    *min_level = pDataProject->noise.min_level;
    *max_level = pDataProject->noise.max_level;
    *min_ct = pDataProject->noise.min_ct;
    *max_ct = pDataProject->noise.max_ct;
    *on = pDataProject->noise.on;

    return true;
}
/**
 * @brief AccessData::getNoise
 * @param prm
 * @return
 */
bool AccessData::getNoise(TYPENOISE * const prm)
{
    prm->time = pDataProject->noise.time;
    prm->min_level = pDataProject->noise.min_level;
    prm->max_level = pDataProject->noise.max_level;
    prm->min_ct = pDataProject->noise.min_ct;
    prm->max_ct = pDataProject->noise.max_ct;
    prm->on = pDataProject->noise.on;

    return true;
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
bool AccessData::getButton( TYPEBUTTON *const prm )
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
bool AccessData::getMotion( uint32_t *const time,
                            uint8_t *const asTVP,
                            uint8_t *const on )
{
    *time = pDataProject->motion.time;
    *asTVP = pDataProject->motion.asTVP;
    *on = pDataProject->motion.on;

    return true;
}
/**
*/
bool AccessData::getMotion( TYPEMOTION *const prm )
{
    prm->on = pDataProject->motion.on;
    prm->asTVP = pDataProject->motion.asTVP;
    prm->time = pDataProject->motion.time;

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
bool AccessData::getOneWeek( const uint8_t day,
                             uint8_t *const plan )
{
    if ( day<MAX_DAY_WEEK )
    {
        *plan = pDataProject->week[day].plan;

        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOneWeek( const uint8_t day, TYPEWEEK *const prm )
{
    if ( day<MAX_DAY_WEEK )
    {
        prm->plan = pDataProject->week[day].plan;

        return true;
    }
    return false;
}
/**
*/
bool AccessData::getSoundValue (const uint8_t numspeaker,
                                uint8_t *const global,
                                uint8_t *const pMass,
                                const uint8_t size)
{
    if ((numspeaker<getMaxSpeaker())&&(size<=getMaxSpeakerLevel()))
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
/**
 * @brief AccessData::countItemHoliday
 * @return
 */
uint32_t AccessData::countItemHoliday()
{
    uint32_t count = 0;
    for ( uint32_t i=0; i<getMaxHoliday(); i++ )
    {
        TYPEHOLIDAY *const pPrm = &pDataProject->holidays[i];
        if ( (pPrm->month<1)||(pPrm->month>12) ||
             (pPrm->day<1)||(pPrm->day>31)
           )
        {
            break;
        }
        count++;
    }
    return count;
}
/**
 * @brief AccessData::countItemPlan
 * @param plan_number
 * @return
 */
uint32_t AccessData::countItemPlan(const uint8_t plan_number)
{
    uint32_t count = 0;

    if ( plan_number < getMaxPlans())
    {
        for ( uint32_t i=0; i<getMaxItemDday(); i++ )
        {
            TYPEPLANITEMS *const pPrm = &pDataProject->day_plans[plan_number].Items[i];

            if ( (pPrm->hour>24) || (pPrm->min>60) ||
                 (pPrm->value_speaker1>100) ||
                 (pPrm->value_speaker2>100)
               )
            {
                break;
            }
            count++;
        }
    }
    return count;
}
/**
 * @brief AccessData::countPlan
 * @return
 */
uint32_t AccessData::countPlan()
{
    uint32_t count = 0;

    for ( uint32_t i=0; i<getMaxPlans(); i++ )
    {
        const uint32_t item = countItemPlan(i);
        if (!item)
        {
            break;
        }
        count++;
    }
    return count;
}

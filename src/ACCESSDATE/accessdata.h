#ifndef ACCESSDATA_H
#define ACCESSDATA_H

#include <stdint.h>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDataStream>
#include "datastructure.h"

namespace ACCESS_DATE{

    class AccessData
    {
    private:
        TYPEPROJECT *pDataProject;
    public:
        explicit AccessData();
        ~AccessData();
        // max value
        inline uint32_t getMaxPlans( void ){return MAX_DAY_PLANS;}
        inline uint32_t getMaxItemDday( void ){return MAX_PLAN_ITEMS;}
        inline uint32_t getMaxHoliday( void ){return MAX_HOLIDAYS;}
        inline uint32_t getMaxSpeaker( void ){return MAX_SPEAKER;}
        inline uint32_t getMaxSpeakerLevel( void ){return MAX_SPEAKER_LEVEL;}
        inline uint32_t getMaxDayWeek( void ){return MAX_DAY_WEEK;}
        // crc16
        bool checkCRC(){return true;}
        // bin
        QByteArray getDataStructure()const
        {
            void *pVoid = pDataProject;
            char *pC = static_cast <char*> (pVoid);
            return QByteArray(pC,sizeof(TYPEPROJECT));
        }
        void setDateStructure(QByteArray &data)
        {
            memcpy(pDataProject,data.constData(),sizeof(TYPEPROJECT));
        }
        //default parametr
        void setDefault(TYPEPROJECT *const pPrg);
        void clear( void ) {
            setDefault(pDataProject);
        }
        // Setters
        bool setNoise(const uint32_t time,
                      const uint8_t min_level,
                      const uint8_t max_level,
                      const uint8_t min_ct,
                      const uint8_t max_ct,
                      const uint8_t on);
        bool setNoise( const TYPENOISE prm );
        void setSetting ( const uint64_t flag,
                          const uint32_t period,
                          const uint32_t delay );
        void setSetting ( const TYPESETTING prm );

        void setVibration( const uint32_t time_lenght,
                           const uint32_t time_pause,
                           const uint32_t intensity,
                           const uint8_t on);
        void setVibration( const TYPEVIBRATION prm );

        void setButton( const uint32_t time,
                        const uint8_t on);
        void setButton( const TYPEBUTTON prm );

        void setAcceleration( const uint32_t time,
                              const uint8_t on);
        void setAcceleration( const TYPEACCELERATION prm );

        void setMotion( const uint8_t on );
        void setMotion( const TYPEMOTION prm );

        bool setOnePlanDay( const uint8_t num_plan,
                            const uint8_t item,
                            const uint8_t hour,
                            const uint8_t min,
                            const uint8_t value_speaker1,
                            const uint8_t value_speaker2 );
        bool setOnePlanDay( const uint8_t num_plan,
                            const uint8_t item,
                            const TYPEPLANITEMS prm );

        bool setOneHoliday( const uint8_t item,
                            const uint8_t month,
                            const uint8_t day,
                            const uint8_t plan);
        bool setOneHoliday( const uint8_t item,
                            const TYPEHOLIDAY prm );
        bool clearHoliday( void );
        bool clearPlans( void );

        bool setOneWeek( const uint8_t day, const uint8_t plan );
        bool setOneWeek(const uint8_t day, const TYPEWEEK *prm );

        bool setSoundValue (const uint8_t numspeaker, const uint8_t global,
                            const uint8_t separate[],
                            const uint8_t size );
        bool setSoundValue (const uint8_t numspeaker, const TYPESOUNDVALUE prm);
        //Getters
        bool getNoise(uint32_t *const time,
                      uint8_t *const min_level,
                      uint8_t *const max_level,
                      uint8_t *const min_ct,
                      uint8_t *const max_ct,
                      uint8_t *const on);
        bool getNoise( TYPENOISE *const prm );
        bool getSetting ( uint64_t *const flag,
                          uint32_t *const period,
                          uint32_t *const delay );
        bool getSetting ( TYPESETTING *const prm );

        bool getVibration( uint32_t *const time_lenght,
                           uint32_t *const time_pause,
                           uint32_t *const intensity,
                           uint8_t *const on);
        bool getVibration( TYPEVIBRATION *const prm );

        bool getButton( uint32_t *const time,
                        uint8_t *const on);
        bool getButton( TYPEBUTTON *const prm );

        bool getAcceleration( uint32_t *const time,
                              uint8_t *const on);
        bool getAcceleration( TYPEACCELERATION *const prm );

        bool getMotion(uint32_t * const time,
                       uint8_t * const asTVP,
                       uint8_t * const on );
        bool getMotion( TYPEMOTION *const prm );

        bool getOnePlanDay( const uint8_t num_plan,
                            const uint8_t item,
                            uint8_t *const hour,
                            uint8_t *const min,
                            uint8_t *const value_speaker1,
                            uint8_t *const value_speaker2 );
        bool getOnePlanDay( const uint8_t num_plan,
                            const uint8_t item,
                            TYPEPLANITEMS *const prm );

        bool getOneHoliday( const uint8_t item,
                            uint8_t *const month,
                            uint8_t *const day,
                            uint8_t *const plan);
        bool getOneHoliday( const uint8_t item,
                            TYPEHOLIDAY *const prm );

        bool getOneWeek(const uint8_t day,
                         uint8_t *const plan );
        bool getOneWeek(const uint8_t day, TYPEWEEK *const prm );

        bool getSoundValue (const uint8_t numspeaker,
                            uint8_t *const global,
                            uint8_t *const pMass,
                            const uint8_t size);
        bool getSoundValue (const uint8_t numspeaker,
                            const uint8_t size,
                            TYPESOUNDVALUE *const prm);
        uint32_t countItemHoliday();
        uint32_t countItemPlan(const uint8_t plan_number);
        uint32_t countPlan();

        friend QDataStream &operator <<( QDataStream &stream,const AccessData &pthis );
        friend QDataStream &operator >>( QDataStream &stream, AccessData &pthis );
        friend QDebug operator<<( QDebug d, const AccessData &pthis );
    };
    /**
     * @brief ACCESS_DATE::AccessData::operator <<
     * @param stream
     * @param pthis
     * @return
     */
    inline QDataStream &operator <<(QDataStream &stream, const AccessData &pthis)
    {
        stream<<pthis.getDataStructure();
        return stream;
    }
    /**
     * @brief ACCESS_DATE::AccessData::operator <<
     * @param d
     * @param pthis
     * @return
     */
    inline QDebug operator<<(QDebug deb, const AccessData &pthis)
    {
        deb<<QString(pthis.getDataStructure());
        return deb;
    }
    /**
     * @brief ACCESS_DATE::AccessData::operator >>
     * @param stream
     * @param pthis
     * @return
     */
    inline QDataStream &operator >>(QDataStream &stream, AccessData &pthis)
    {

        QByteArray data;
        stream>>data;
        pthis.setDateStructure(data);
        return stream;
    }

}

#endif // CREATEDAYPLAN_H

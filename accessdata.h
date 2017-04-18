#ifndef ACCESSDATA_H
#define ACCESSDATA_H

#include <stdint.h>
//#include <QByteArray>


namespace ACCESS_DATE{


    enum MAX_PROJRCT{
        MAX_SPEAKER = 2,        // динамика
        MAX_SPEAKER_LEVEL = 5,       // уровней громкости для 5 мелодий
        MAX_PLAN_ITEMS = 24,    // планов в день
        MAX_DAY_PLANS = 24,     // дней с заданными планами
        MAX_HOLIDAYS = 120,     // праздничный дней
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
    // план на неделю.
    struct TYPEWEEK
    {
    	uint8_t day;
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
	uint32_t	time_lenght;
	uint32_t	time_pause;
	uint32_t 	intensity;
	uint8_t 	on;
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
        TYPEHOLIDAY 		holidays[MAX_HOLIDAYS];	    //праздничные дни
		TYPEDAY 		    day_plans[MAX_DAY_PLANS];	// дни с планами
		TYPEWEEK			week;					    // неделя с планами
		TYPEALLSOUND	 	sound_value;				//  настройка громкости
		TYPENOISE 			noise;					    // датчик шума
		TYPEMOTION		    motion;				        // датчик движения
		TYPEACCELERATION	acceleration;				// ускорение звука.
		TYPEBUTTON		    button;					    // кнопка
		TYPEVIBRATION		vibration;				    // двигатель вибрации
		TYPESETTING		    setting;					// общие настройки.
	};
    #pragma pack(pop)

    /**
    *
    */
	class AccessData
    {
        TYPEPROJECT *pDataProject;
    public:
        explicit AccessData();
		~AccessData();
        //
        //QByteArray getDateStructure() const;
        //void setDateStructure(QByteArray data);
        //
        inline uint32_t getMaxPlans( void ){return MAX_DAY_PLANS;}
        inline uint32_t getMaxItemDday( void ){return MAX_PLAN_ITEMS;}
        inline uint32_t getMaxHoliday( void ){return MAX_HOLIDAYS;}
        inline uint32_t getMaxSpeaker( void ){return MAX_SPEAKER;}
        inline uint32_t getMaxSpeakerLevel( void ){return MAX_SPEAKER_LEVEL;}
        //default parametr
        void setDefault(TYPEPROJECT *const pPrg);
        // Setters
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

        void setOneWeek( const uint8_t day, const uint8_t plan );
        void setOneWeek( const TYPEWEEK prm );

        bool setSoundValue (const uint8_t numspeaker,
                            const uint8_t global,
                            const uint8_t separate[],
                            const uint8_t size );
        bool setSoundValue (const uint8_t numspeaker, const TYPESOUNDVALUE prm);
        //Getters
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

        bool getMotion( uint8_t *const on );
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

        bool getOneWeek( uint8_t *const day,
                         uint8_t *const plan );
        bool getOneWeek( TYPEWEEK *const prm );

        bool getSoundValue (const uint8_t numspeaker,
                            const uint8_t size,
                            uint8_t *const global,
                            uint8_t *const pMass);
        bool getSoundValue (const uint8_t numspeaker,
                            const uint8_t size,
                            TYPESOUNDVALUE *const prm);
    };
};

#endif // CREATEDAYPLAN_H

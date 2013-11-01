// projektlokala headerfiler
#include "kattistime.h"     
#include "julian.h"         
#include "gregorian.h"      

// STL headerfiler
#include <iostream>
#include <assert.h>             // assert(b) ger felmeddelande om b falsk
#include <ctime>


// Obs att testerna förutsätter att dina klasser returnerar
// månader och dagar indexerade från ETT. Testerna är inte
// fullständiga


int main()
{
    using namespace lab2;        // Exponera funktionalitet i namnrymden lab2

    ////////////////////////////////////////////////////////////
    // Sätt tiden. OBS skicka inte nedanstående kod till kattis
    time_t mytime;
    time(&mytime);
    set_k_time(mytime);
    ////////////////////////////////////////////////////////////

    enum WEEK_DAY_NR
        {
            SUNDAY = 0,
            MONDAY,
            TUESDAY,
            WEDNESDAY,
            THURSDAY,
            FRIDAY,
            SATURDAY,
            DAYS_PER_WEEK
        };
        
        enum MONTH_NR
        {
            JANUARY = 1,
            FEBRUARY,
            MARCH,
            APRIL,
            MAY,
            JUNE,
            JULY,
            AUGUST,
            SEPTEMBER,
            OCTOBER,
            NOVEMBER,
            DECEMBER,
            MONTHS_PER_YEAR = DECEMBER
        };

    
    Julian tj;                  // ok: defaultkonstruktor ger dagens datum
    Gregorian gtoday;           // ok för gregorian också
    std::cout << "Idag är det " << gtoday << std::endl;
    assert(tj - gtoday == 0);
    Gregorian tg(2006, 10, 31); // ok: sätt datum explicit
                                // följande fungerar också:
                                // Gregorian(2000, Gregorian::October, 31)
    Date &j = tj;               // åtkomst av funktioner genom Dates interface
    Date &g = tg;       
    Date &today = gtoday;

    {
      Julian j1;
      Date & d1 = j1;
      Julian j2(d1);
      Date * dp = &j2;
      Julian j3(dp);
    }
     
    time_t tp;
    time(&tp);
    struct tm *t = gmtime(&tp);
    int year  = t->tm_year + 1900;
    int month = t->tm_mon + 1;      // månaderna och dagarna
    int day   = t->tm_mday;         // indexerade från ETT

    std::cout << "Testing constructors..." << std::endl;
    assert(today.year() == year &&          // rätt initierad
           today.month() == month &&
           today.day() == day);
    assert(g.year() == 2006 &&              // rätt initierad
           g.month() == 10 &&
           g.day() == 31);                  // obs! ettindexerade
    
    std::cout << "Testing access..." << std::endl;
    assert(g.days_per_week() == 7);         // rätt antal dagar per vecka
    assert(j.days_per_week() == 7);         // rätt antal dagar per vecka
    assert(g.days_this_month() == 31);      // rätt antal dagar denna månad
    assert(g.months_per_year() == 12);      // rätt antal månader per år
    assert(j.months_per_year() == 12);      // rätt antal månader per år
    assert(g.week_day() == 2); // rätt veckodag

    std::cout << "Testing manipulation..." << std::endl;
    ++g;                                    // prefix ökning
    assert(g.week_day() == 3); // rätt veckodag
    --g;                                    // prefix minskning
    assert(g.week_day() == 2); // rätt veckodag
    g += 2;                                 // lägg till två dagar
    assert(g.week_day() == 4); // rätt veckodag
    g -= 3;                     // dra bort tre dagar
    g.add_month();              // lägg till en månad
    g.add_month(-1);            // dra bort en månad    
    g.add_year(10);             // lägg till tio år.
    
    std::cout << "Testing miscellaneous functions..." << std::endl;
    Julian jj(tj);              // kopieringskonstruktor
    const Gregorian gg;
    gg.year();                  // gg konstant, läsa går bra
    g = gg;                     // tilldelning
    if(g == gg ||               // jämförelse
       g != gg ||               // jämförelse 
       g < gg ||                // jämförelse 
       g >= gg)                 // jämförelse 
        {}
    
    std::cout << "Testing boundary violations";
    Gregorian temp(1900, 1, 1);
    Date &d = temp;

    // loopa över dagar och kolla att inga gränser över/underskrids
    for(int i = 0; i < 100000; ++i, ++d)
        {
            if(!(i % 5000))        // utskrift på framsteg
                {
                    std::cout << ".";
                    flush(std::cout);
                }
            int m[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if(d.year() >= 1900 && 
               d.month() >= 1 && d.month() <= 12 &&
               d.day() >= 1 && d.day() <= m[d.month() - 1])
                {}
            else
                {
                    std::cout << std::endl;
                    std::cout << "boundary checks failed: " << std::endl;
                    std::cout << d.year() << ", "
                              << d.month() << ", "
                              << d.day() << std::endl;
                    return 1;
                }
        }
    std::cout << std::endl;

    std::cout << "Testing leap years..." << std::endl;
    // testa om skottåren för sekelskiften fungerar som de ska
    for(int y = 1958; y < 2500; y += 100)
        {
            Gregorian temp(y, 2, 28);
            Date &e = temp;
            ++e;                                   // lägg till en dag
            if((y % 400 == 0 && e.day() == 29) ||  // skottdag (obs! ETTindexerad)
               (y % 400 != 0 && e.day() == 1))     // ej skottdag
                {}
            else
                {
                    std::cout << std::endl << "leap day test failed: " << std::endl;
                    std::cout << e.year() << ", "
                              << e.month() << ", "
                              << e.day() << std::endl;
                    return 1;
                }
        }
    
    // More handcrafted tests.
    std::cout << "Testing more miscellaneous methods." << std::endl;

    year = 2007;
    month = 1;
    day = 1;

    Gregorian test_g(year,month,day);

    // JANUARY
    std::cout << "\nStart - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;
    assert(test_g.year() == 2007);
    assert(test_g.week_day() == MONDAY);
    assert(test_g.week_day_name() == "monday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == JANUARY);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "january");
    assert(test_g.months_per_year() == 12);

    // FEBRUARY
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == THURSDAY);
    assert(test_g.week_day_name() == "thursday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == FEBRUARY);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 28);
    assert(test_g.month_name() == "february");
    assert(test_g.months_per_year() == 12);

    // MARCH
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == THURSDAY);
    assert(test_g.week_day_name() == "thursday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == MARCH);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "march");
    assert(test_g.months_per_year() == 12);

    // APRIL
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == SUNDAY);
    assert(test_g.week_day_name() == "sunday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == APRIL);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 30);
    assert(test_g.month_name() == "april");
    assert(test_g.months_per_year() == 12);

    // MAY
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == TUESDAY);
    assert(test_g.week_day_name() == "tuesday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == MAY);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "may");
    assert(test_g.months_per_year() == 12);

    // JUNE
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == FRIDAY);
    assert(test_g.week_day_name() == "friday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == JUNE);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 30);
    assert(test_g.month_name() == "june");
    assert(test_g.months_per_year() == 12);

    // JULY
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == SUNDAY);
    assert(test_g.week_day_name() == "sunday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == JULY);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "july");
    assert(test_g.months_per_year() == 12);

    // AUGUST
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == WEDNESDAY);
    assert(test_g.week_day_name() == "wednesday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == AUGUST);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "august");
    assert(test_g.months_per_year() == 12);

    // SEPTEMBER
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == SATURDAY);
    assert(test_g.week_day_name() == "saturday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == SEPTEMBER);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 30);
    assert(test_g.month_name() == "september");
    assert(test_g.months_per_year() == 12);

    // OCTOBER
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == MONDAY);
    assert(test_g.week_day_name() == "monday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == OCTOBER);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "october");
    assert(test_g.months_per_year() == 12);

    // NOVEMBER
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == THURSDAY);
    assert(test_g.week_day_name() == "thursday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == NOVEMBER);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 30);
    assert(test_g.month_name() == "november");
    assert(test_g.months_per_year() == 12);

    // DECEMBER
    test_g.add_month();
    std::cout << "Added Month - Current date is now: " << test_g << "| " << test_g.week_day_name() << "-" << test_g.week_day() << std::endl;

    assert(test_g.year() == 2007);
    assert(test_g.week_day() == SATURDAY);
    assert(test_g.week_day_name() == "saturday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == DECEMBER);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "december");
    assert(test_g.months_per_year() == 12);

    // JANUARY 2008
    test_g.add_year();
    std::cout << "Added year - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 2008);
    assert(test_g.week_day() == MONDAY);
    assert(test_g.week_day_name() == "monday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == DECEMBER);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "december");
    assert(test_g.months_per_year() == 12);

    std::cout << std::endl << "All tests were successful." << std::endl;


    // följande ska inte gå att kompilera
#if 0
    gg.leap_year();             // fel: kan inte komma åt protected/private medlem
    gg = g;                     // fel: kan inte tilldela konstant
    ++gg;                       // fel: kan inte ändra på en konstant
#endif

    return 0;
}


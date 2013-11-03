// projektlokala headerfiler
#include "kattistime.h"     
#include "julian.h"         
#include "gregorian.h"      

// STL headerfiler
#include <iostream>
#include <assert.h>             // assert(b) ger felmeddelande om b falsk
#include <ctime>
#include <math.h>


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
            MONDAY = 1,
            TUESDAY,
            WEDNESDAY,
            THURSDAY,
            FRIDAY,
            SATURDAY,
            SUNDAY,
            DAYS_PER_WEEK = SUNDAY
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
	
	std::cout << tj.mod_julian_day() << std::endl;
	std::cout << gtoday.mod_julian_day() << std::endl;
	
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
    std::cout << "mod julian day nr: " << floor(test_g.mod_julian_day()) << std::endl;
	assert(test_g.year() == 2007);
    assert(test_g.week_day() == MONDAY);
    assert(test_g.week_day_name() == "monday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == JANUARY);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "january");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 54101);

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
    assert(floor(test_g.mod_julian_day()) == 54132);

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
    assert(floor(test_g.mod_julian_day()) == 54160);

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
    assert(floor(test_g.mod_julian_day()) == 54191);

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
    assert(floor(test_g.mod_julian_day()) == 54221);

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
    assert(floor(test_g.mod_julian_day()) == 54252);

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
    assert(floor(test_g.mod_julian_day()) == 54282);

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
    assert(floor(test_g.mod_julian_day()) == 54313);

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
    assert(floor(test_g.mod_julian_day()) == 54344);

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
    assert(floor(test_g.mod_julian_day()) == 54374);

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
    assert(floor(test_g.mod_julian_day()) == 54405);

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
    assert(floor(test_g.mod_julian_day()) == 54435);

    // DECEMBER 2008
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
    assert(floor(test_g.mod_julian_day()) == 54801);

    // FEBRUARY 2009    
    test_g.add_month(2);
    std::cout << "Added 3 months - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 2009);
    assert(test_g.week_day() == SUNDAY);
    assert(test_g.week_day_name() == "sunday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == FEBRUARY);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 28);
    assert(test_g.month_name() == "february");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 54863);

    // MAY 2009    
    test_g.add_month(3);
    std::cout << "Added 3 months - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 2009);
    assert(test_g.week_day() == FRIDAY);
    assert(test_g.week_day_name() == "friday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == MAY);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "may");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 54952);

    // MAY 2009    
    test_g++;
    test_g++;
    test_g++;

    std::cout << "Added 3 days - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 2009);
    assert(test_g.week_day() == MONDAY);
    assert(test_g.week_day_name() == "monday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == MAY);
    assert(test_g.day() == 4);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "may");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 54955);

    std::cout << std::endl << "All tests were successful." << std::endl;

    // MAY 2009  
    Gregorian tmp = test_g;  
    test_g += 15;

    std::cout << "Added 15 days - Current date is now: " << test_g << std::endl;

    // testing operators
    assert(tmp != test_g);
    assert((tmp == test_g) == false);
    assert(test_g != tmp);
    assert(tmp < test_g);
    assert(test_g > tmp);
    assert(tmp <= test_g);
    assert(test_g >= tmp);
    assert(test_g - tmp == 15);

    assert(test_g.year() == 2009);
    assert(test_g.week_day() == TUESDAY);
    assert(test_g.week_day_name() == "tuesday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == MAY);
    assert(test_g.day() == 19);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "may");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 54970);

    // MAY 2009  
    test_g -= 15;
    std::cout << "Subtracted 15 days - Current date is now: " << test_g << std::endl;
    
    // testing operators
    assert(tmp == test_g);
    assert((tmp != test_g) == false);
    assert((test_g != tmp) == false);
    assert((tmp > test_g) == false);
    assert((tmp < test_g) == false);
    assert((test_g < tmp) == false);
    assert((test_g > tmp) == false);
    assert(test_g == tmp);
    assert(tmp <= test_g);
    assert(test_g >= tmp);
    assert(tmp >= test_g);
    assert(test_g <= tmp);
    assert(tmp - test_g == 0);

    assert(test_g.year() == 2009);
    assert(test_g.week_day() == MONDAY);
    assert(test_g.week_day_name() == "monday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == MAY);
    assert(test_g.day() == 4);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "may");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 54955);

    // MAY 2012    
    test_g.add_year(3);

    std::cout << "Added 3 years - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 2012);
    assert(test_g.week_day() == FRIDAY);
    assert(test_g.week_day_name() == "friday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == MAY);
    assert(test_g.day() == 4);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "may");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 56051);

    // FEBRUARY 2012: LEAP YEAR    
    test_g.add_month(-3);
    test_g += 25;

    std::cout << "Subtracted 3 months and added 25 days - LEAP DAY CHECK - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 2012);
    assert(test_g.week_day() == WEDNESDAY);
    assert(test_g.week_day_name() == "wednesday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == FEBRUARY);
    assert(test_g.day() == 29);
    assert(test_g.days_this_month() == 29);
    assert(test_g.month_name() == "february");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 55986);

    // FEBRUARY 2008: LEAP YEAR    
    test_g.add_year(-4);

    std::cout << "Subtracted 4 years - LEAP DAY CHECK - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 2008);
    assert(test_g.week_day() == FRIDAY);
    assert(test_g.week_day_name() == "friday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == FEBRUARY);
    assert(test_g.day() == 29);
    assert(test_g.days_this_month() == 29);
    assert(test_g.month_name() == "february");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 54525);

    // FEBRUARY 2005: NOT A LEAP YEAR    
    test_g.add_year(-3);

    std::cout << "Subtracted 3 years - NOT LEAP YEAR - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 2005);
    assert(test_g.week_day() == MONDAY);
    assert(test_g.week_day_name() == "monday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == FEBRUARY);
    assert(test_g.day() == 28);
    assert(test_g.days_this_month() == 28);
    assert(test_g.month_name() == "february");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 53429);

    // FEBRUARY 2005: NOT A LEAP YEAR    
    test_g.add_year(100);
	Gregorian new_g(2105,2,28);

    std::cout << "Added 100 years - NOT LEAP YEAR - Current date is now: " << test_g << std::endl;
	std::cout << "mod julian day nr (old): " << floor(test_g.mod_julian_day()) << std::endl;
	std::cout << "mod julian day nr (new): " << new_g.mod_julian_day() << std::endl;
	
    assert(test_g.year() == 2105);
    assert(test_g.week_day() == SATURDAY);
    assert(test_g.week_day_name() == "saturday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == FEBRUARY);
    assert(test_g.day() == 28);
    assert(test_g.days_this_month() == 28);
    assert(test_g.month_name() == "february");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 89953);

    // FEBRUARY 2005: NOT A LEAP YEAR    
    test_g.add_year(-200);

    std::cout << "Subtracted 200 years - NOT LEAP YEAR - Current date is now: " << test_g << std::endl;
	std::cout << "mod julian day nr (old): " << floor(test_g.mod_julian_day()) << std::endl;

    assert(test_g.year() == 1905);
    assert(test_g.week_day() == TUESDAY);
    assert(test_g.week_day_name() == "tuesday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == FEBRUARY);
    assert(test_g.day() == 28);
    assert(test_g.days_this_month() == 28);
    assert(test_g.month_name() == "february");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 16904);

    // FEBRUARY 2005: NOT A LEAP YEAR    
    
	test_g.add_year(-5);
    test_g.add_month(-1);
    test_g -= 25;
    std::cout << "Subtracted 5 years, subtracted 1 month, subtracted 25 days - NOT LEAP YEAR - Current date is now: " << test_g << std::endl;
    std::cout << "mod julian day nr (old): " << floor(test_g.mod_julian_day()) << std::endl;

    assert(test_g.year() == 1900);
    assert(test_g.week_day() == WEDNESDAY);
    assert(test_g.week_day_name() == "wednesday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == JANUARY);
    assert(test_g.day() == 3);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "january");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 15022);

    // FEBRUARY 2005: NOT A LEAP YEAR    
    
    test_g--;
    test_g--;
    test_g--;

    std::cout << "Subtracted 3 days - NOT LEAP YEAR - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 1899);
    assert(test_g.week_day() == SUNDAY);
    assert(test_g.week_day_name() == "sunday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == DECEMBER);
    assert(test_g.day() == 31);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "december");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 15019);

    // FEBRUARY 2005: NOT A LEAP YEAR    

    test_g.add_month(17);

    std::cout << "Added 17 months - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 1901);
    assert(test_g.week_day() == SUNDAY);
    assert(test_g.week_day_name() == "sunday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == JUNE);
    assert(test_g.day() == 2);
    assert(test_g.days_this_month() == 30);
    assert(test_g.month_name() == "june");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 15537);

    // FEBRUARY 2005: NOT A LEAP YEAR    

    test_g.add_month(-17);

    std::cout << "Subtracted 17 months - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 1900);
    assert(test_g.week_day() == TUESDAY);
    assert(test_g.week_day_name() == "tuesday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == JANUARY);
    assert(test_g.day() == 2);
    assert(test_g.days_this_month() == 31);
    assert(test_g.month_name() == "january");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 15021);

    // FEBRUARY 2005: NOT A LEAP YEAR    

    test_g += 29;
    test_g.add_month(-17);

    std::cout << "Subtracted 17 months and added 29 days - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 1898);
    assert(test_g.week_day() == THURSDAY);
    assert(test_g.week_day_name() == "thursday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == SEPTEMBER);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 30);
    assert(test_g.month_name() == "september");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 14533);

    // FEBRUARY 2005: NOT A LEAP YEAR    

    test_g += 366;

    std::cout << "Added 366 days - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 1899);
    assert(test_g.week_day() == SATURDAY);
    assert(test_g.week_day_name() == "saturday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == SEPTEMBER);
    assert(test_g.day() == 2);
    assert(test_g.days_this_month() == 30);
    assert(test_g.month_name() == "september");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 14899);

    // FEBRUARY 2005: NOT A LEAP YEAR    

    test_g -= 366;

    std::cout << "Subtracted 366 days - Current date is now: " << test_g << std::endl;

    assert(test_g.year() == 1898);
    assert(test_g.week_day() == THURSDAY);
    assert(test_g.week_day_name() == "thursday");
    assert(test_g.days_per_week() == 7);
    assert(test_g.month() == SEPTEMBER);
    assert(test_g.day() == 1);
    assert(test_g.days_this_month() == 30);
    assert(test_g.month_name() == "september");
    assert(test_g.months_per_year() == 12);
    assert(floor(test_g.mod_julian_day()) == 14533);



    // From the lab-PM
    Gregorian gn; // dagens datum
    Julian jn; // också dagens datum
    std::cout << "Today it is " << gn << " gregorian and " << jn << " julian";
    if (gn - jn == 0) std::cout << ". It is the same date" << std::endl;
    gn = jn;
    if (gn - jn == 0) std::cout << "It is still the same date" << std::endl;

    // From the lab-PM
    Gregorian gn1(2013, 11, 1); // dagens datum
    Julian jn1(2013, 10, 19); // också dagens datum
    std::cout << "Today it is " << gn1 << " gregorian and " << jn1 << " julian";
    if (gn1 - jn1 == 0) std::cout << ". It is the same date" << std::endl;
    gn = jn;
    if (gn1 - jn1 == 0) std::cout << "It is still the same date" << std::endl;

    jn1++;
    gn1++;

    std::cout << "Today it is " << gn1 << " gregorian and " << jn1 << " julian";
    if (gn1 - jn1 == 0) std::cout << ". It is the same date" << std::endl;
    gn = jn;
    if (gn1 - jn1 == 0) std::cout << "It is still the same date" << std::endl;

    std::cout << std::endl << "All tests were successful." << std::endl;

    jn1--;
    gn1--;

    std::cout << "Today it is " << gn1 << " gregorian and " << jn1 << " julian";
    if (gn1 - jn1 == 0) std::cout << ". It is the same date" << std::endl;
    gn = jn;
    if (gn1 - jn1 == 0) std::cout << "It is still the same date" << std::endl;

    std::cout << std::endl << "All tests were successful." << std::endl;

    // följande ska inte gå att kompilera
#if 0
    gg.leap_year();             // fel: kan inte komma åt protected/private medlem
    gg = g;                     // fel: kan inte tilldela konstant
    ++gg;                       // fel: kan inte ändra på en konstant
#endif

    return 0;
}


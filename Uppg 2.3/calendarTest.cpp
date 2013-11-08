/// Description: Test file for Calendar.
///
/// Authors: Martin Pettersson, Christoffer Wiss
///
/// Version: 2013-11-06

#include "kattistime.h"
#include "gregorian.h"
#include "julian.h"
#include "calendar.h"
#include <assert.h>
#include <iostream>
using namespace lab2;

int main()
{
    /** Set time to current time **/
	time_t mytime;
	time(&mytime);
	set_k_time(mytime);
    /*******************************/
	
    // Test from lab
    std::cout << "----------------------------------------" << std::endl;
    Calendar<Gregorian> cal;
    cal.set_date(2000, 12, 2);
    cal.add_event("Basketträning", 4, 12, 2000);
    cal.add_event("Basketträning", 11, 12, 2000);
    cal.add_event("Nyårsfrukost", 1, 1, 2001);
    cal.add_event("Första advent", 1); // år = 2000, månad = 12
    cal.add_event("Vårdagjämning", 20, 3); // år = 2000
    cal.add_event("Julafton", 24, 12);
    cal.add_event("Kalle Anka hälsar god jul", 24); // också på julafton
    cal.add_event("Julafton", 24); // En likadan händelse samma datum ska
    // ignoreras och inte sättas in i kalendern
    cal.add_event("Min första cykel", 20, 12, 2000);
    cal.remove_event("Basketträning", 4);
    std::cout << cal; // OBS! Vårdagjämning och första advent är
    // före nuvarande datum och skrivs inte ut
    std::cout << "----------------------------------------" << std::endl;
    cal.remove_event("Vårdagjämning", 20, 3, 2000);
    cal.remove_event("Kalle Anka hälsar god jul", 24, 12, 2000);
    cal.set_date(2000, 11, 2);
    if (! cal.remove_event("Julafton", 24)) {
    std::cout << " cal.remove_event(\"Julafton\", 24) tar inte"<< std::endl
    << " bort något eftersom aktuell månad är november" << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
    std::cout << cal;
    std::cout << std::endl;

    Calendar<Julian> cal1;
    Calendar<Gregorian> cal2;

    cal2.set_date(1879,5,2);
    cal2.add_event("T:1879-5-2");
    cal2.add_event("T:1879-5-27", 27);
    cal2.add_event("T:1879-1-27", 27,1);
    cal2.add_event("B", 27,1,1870);
    cal2.add_event("T:1876-1-27", 27,1,1870);
    cal2.add_event("A", 27,1,1870);
    cal2.add_event("T:1920-1-27", 27,1,1920);
    cal2.add_event("T:1860-5-2",2,5,1860);
    cal2.add_event("Testing on a friday?", 27,1,2100);
	cal2.add_event("Testing on a friday?", 28,1,1920);

    cal2.set_date(1860,5,2);
	std::cout << cal2 << std::endl;

    cal2.remove_event("T:1879-5-2");
    cal2.remove_event("T:1879-5-27",27);
    cal2.remove_event("T:1870-1-27", 27,1,1870);
    cal2.remove_event("T:1870-1-27", 27,1,1870);
    cal2.remove_event("T:1870-1-27B", 27,1,1870);
    cal2.remove_event("T:1920-1-27", 27,1,1920);
    cal2.remove_event("Testing on a friday?", 27,1,2100);
    cal2.remove_event("Testing on a friday?", 28,1,1920);

    cal2.set_date(1860,5,1);
    std::cout << cal2 << std::endl;
	
	cal1 = cal2;
	cal1.remove_event("Testing on a friday?",28,1,1920);
	cal1.add_event("Testing on Christmas 1",20,12,1989);
	cal1.add_event("Testing on Christmas 2",20,12,1989);
	if(!cal1.add_event("Testing on Christmas1",25,12,1989)) std::cout << "failed" << std::endl;
	if(!cal1.add_event("Testing on Christmas2",25,12,2089)) std::cout << "failed" << std::endl;
	if(!cal1.add_event("Testing on Christmas1",30,12,1989)) std::cout << "failed" << std::endl;
	cal1.add_event("Testing");

	std::cout << cal1 << std::endl;

    // Testing assignment
    cal2 = cal1;
    assert(cal1.remove_event("Testing on a friday?",28,1,1920) == false);
    assert(cal2.remove_event("Testing on a friday?",10,2,1920) == false);
    assert(cal1.add_event("Testing on a friday?",28,1,1920) == true);
    assert(cal2.add_event("Testing on a friday?",10,2,1920) == true);
    assert(cal1.remove_event("Testing on a friday?",28,1,1920) == true);
    assert(cal2.remove_event("Testing on a friday?",10,2,1920) == true);
    assert(cal1.add_event("Testing on Christmas 1",20,12,1989) == false);
    assert(cal2.add_event("Testing on Christmas 1",2,1,1990) == false);
    assert(cal1.remove_event("Testing on Christmas 1",20,12,1989) == true);
    assert(cal2.remove_event("Testing on Christmas 1",2,1,1990) == true);
    assert(cal1.add_event("Testing on Christmas 1",20,12,1989) == true);
    assert(cal2.add_event("Testing on Christmas 1",2,1,1990) == true);

    Calendar<Julian> cal_j;
    Calendar<Gregorian> cal_g;

	return 0;
}

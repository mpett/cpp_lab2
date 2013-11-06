/// Description: Implemenation of Date for Julian Calender.
/// 
/// Authors: Martin Pettersson, Christoffer Wiss
///
/// Version: 2013-11-06

#pragma once

#include <ostream>
#include <string>
#include "kattistime.h"
#include "date.h"
using std::ostream;
using std::string;

namespace lab2
{
	class Julian : public Date
	{
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
		
		public:
			// Destructor.
            ~Julian(){}
			
			// Default Constructor (sets date to current date).
			Julian();
			
			// Constructor.
			Julian(int year, int month, int day);
			
			// Copy constructor.
			Julian(const Date &date);
			
			// Construct Julian from Date pointer.
			Julian(const Date *date);
			
			// Postfix ++ operator; Increases date by one day.
            Julian operator++(int);
			
			// Postfix -- operator; Decreases date by one day.
            Julian operator--(int);
			
			// Returns the nr of days for current month.
			int days_this_month() const;
			
			// Returns the name of the current week day.
			string week_day_name() const;
			
			// Returns the name of the current month.
			string month_name() const;

			// Assignment operator. Assigns from date reference. Returns reference to a new date object.
			Julian& operator=(const Date& date);
			
			// Adds n to current year, ensures that new date is valid.
			//
			// n : nr of years to add/subtract
			void add_year(int n = 1);
			
			// Adds n to current month, ensures that new date is valid.
			//
			// n : nr of months to add/subtract
			void add_month(int n = 1);
			
			// Sets the date's year, month and day to input.
			void set_date(int year, int month, int day);
			
			// Returns Julian day number.
            int mod_julian_day() const;

		private:
			// Nr of days in a default month, used when one cannot get the same date after adding a month (ex. 31 Jan + 1 month != 31 February)
			static const int GENERIC_MONTH_D = 30; 
			int daysPerMonths_[MONTHS_PER_YEAR];
            static const int MIN_YEAR = 1858; // Smallest year that can be set
            static const int MAX_YEAR = 2558; // Largest year that can be set
			
			// Checks if current date is a valid Julian date.
			void checkValidDate();
			
			// Sets days per month to standard days per month 
			// if non-leap year or with 29 days in February if leap-year.
			void initializeDaysPerMonths(bool leapYear);
			
			// Returns true if leap year (year dividable by 4) otherwise false.
			bool isLeapYear();
			
			// Set Julian date (using a quite dreadful formula) 
			// given a Julian day number.
            void setDateFromJulian(double julian);

			// Used for converting a Gregorian date (given by current time) to a Julian Date.
			void convertFromGregorianToJulian(int year, int month, int monthDay);
			
			// Updates the nr of days in february.
			// (29 if leapyear, 28 otherwise)
			void updateFebruaryDays(bool leapYear);
			
			// Calculates the current weekday given year, month and day of the month.
			int calculateCurrentWeekday(int year, int month, int monthDay);
	};
}

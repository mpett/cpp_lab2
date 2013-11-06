/// Description: A calendar of specified type.
///              Type must be derived from Date.
///              A calendar maintains events_ and is capable of outputting.
///
/// Authors: Martin Pettersson, Christoffer Wiss
///
/// Version: 2013-11-06

#pragma once

#include "date.h"
#include <vector>
#include <string>
#include <exception>
#include <type_traits>
#include <ostream>
#include <algorithm>
using std::ostream;
using std::vector;
using std::pair;
using std::string;

namespace lab2
{
	template<class T>
    class Calendar;

	template < class T >
	ostream & operator <<(ostream &, const Calendar<T> &);
	
    template<class T>
    class Calendar
    {	
    public:
        // Make Calendar template classes with other type friend with this template class,
        // thus allowing access to private members of Calendar of other date type.
		template<class G> friend class Calendar; 
		
        // Make ostream friend to this class to allow access to private members
		friend ostream & operator<< <>(ostream &, const Calendar<T> &);

        // Default constructor
        Calendar();
		
        // Copy constructor (of calendar with same & other type)
		Calendar(const Calendar<T> &calendar);
		template<class G> Calendar<T>(const Calendar<G> &calendar);
		
		// Help function to constructor.
		void initialize(std::true_type);

        // Destructor
        ~Calendar();

        // Adds an even to the calender at currentDate_.
        // If event is unique (i.e. does an event with same description at input date) adds event and returns true
        // otherwise returns false.
        bool add_event(string desc);

        // Adds an even to the calender at input date. Fills in month & year from currentDate_.
        // If event is unique (i.e. does an event with same description at input date) adds event and returns true
        // otherwise returns false.
        bool add_event(string desc, int day);

        // Adds an even to the calender at input date. Fills in year from currentDate_.
        // If event is unique (i.e. does an event with same description at input date) adds event and returns true
        // otherwise returns false.
        bool add_event(string desc, int day, int month);

        // Adds an even to the calender at input date.
        // If event is unique (i.e. does an event with same description at input date) adds event and returns true
        // otherwise returns false.
        bool add_event(string desc, int day, int month, int year);

        // Sets the currentDate_ to input.
        // If date is invalid, returns false otherwise returns true.
        bool set_date(int year, int month, int day);

        // Tries to remove an event at currentDate_.
        // If successful, returns true otherwise false.
        bool remove_event(string desc);

        // Tries to remove an event matching input. Fills in month & year from currentDate_.
        // If successful, returns true otherwise false.
        bool remove_event(string desc, int day);

        // Tries to remove an event matching input. Fills in year from currentDate_.
        // If successful, returns true otherwise false.
        bool remove_event(string desc, int day, int month);

        // Tries to remove an event matching input.
        // If successful, returns true otherwise false.
        bool remove_event(string desc, int day, int month, int year);

		// Assigns the calendar to input calendar of same type.
		Calendar<T> & operator=(const Calendar<T> &calendar);
		
        // Assigns the calendar to input calendar.
        // Converts dates of events to corresponding calendar type.
        template<class G> Calendar<T> &operator=(const Calendar<G> &calendar);

        // Helper function to assignment.
		template<class G> Calendar<T> &assignment_helper(const Calendar<G> &calendar, std::true_type);
		

    private:
        Date *currentDate_;
        vector<pair<T,string>> events_;
    }; // class Calendar
	
    // Puts events after currentDate to to outputstream object.
    // Each event has the following format: DATE : Description
	template<class T>
    ostream& operator<<(ostream& os, const Calendar<T> &calendar)
	{
        for(size_t i = 0; i < calendar.events_.size(); i++)
		{
			// Only output dates which are after (>) current 
            if(calendar.events_[i].first > *(calendar.currentDate_))
			{
                os << calendar.events_[i].first << " : " << calendar.events_[i].second << std::endl;
			}
		}
		return os;
	}

    // Destructor.
    template<class T> Calendar<T>::~Calendar()
    {
        delete currentDate_;
        events_.clear();
    }

    // Default constructor.
    template<class T> Calendar<T>::Calendar()
    {
        initialize(typename std::is_base_of<Date,T>::type()); // Make sure that T is derived from Date (will otherwise generate a compile error)
    }
	
    // Copy constructor for Calendar of same type.
    template<class T> Calendar<T>::Calendar(const Calendar<T> &cal)
	{
        initialize(typename std::is_base_of<Date,T>::type()); // Make sure that T is derived from Date (will otherwise generate a compile error)
		operator=(cal);
	}
	
    // Copy constructor for Calendar of other type.
	template<class T> 
	template<class G> 
    Calendar<T>::Calendar(const Calendar<G> &cal)
	{
        initialize(typename std::is_base_of<Date,G>::type()); // Make sure that G is derived from Date (will otherwise generate a compile error)
		operator=(cal);
	}
	
    // Help function to constructor.
	template<class T> void Calendar<T>::initialize(std::true_type)
	{
        currentDate_ = new T();
	}

    // Adds an even to the calender at currentDate_.
    // If event is unique (i.e. does an event with same description at input date) adds event and returns true
    // otherwise returns false.
    template<class T> bool Calendar<T>::add_event(string desc)
    {
        return add_event(desc,currentDate_->day(),currentDate_->month(),currentDate_->year());
    }

    // Adds an even to the calender at input date. Fills in month & year from currentDate_.
    // If event is unique (i.e. does an event with same description at input date) adds event and returns true
    // otherwise returns false.
    template<class T> bool Calendar<T>::add_event(string desc, int day)
    {
        return add_event(desc,day,currentDate_->month(),currentDate_->year());
    }

    // Adds an even to the calender at input date. Fills in year from currentDate_.
    // If event is unique (i.e. does an event with same description at input date) adds event and returns true
    // otherwise returns false.
    template<class T> bool Calendar<T>::add_event(string desc, int day, int month)
    {
        return add_event(desc,day,month,currentDate_->year());
    }

    // Adds an even to the calender at input date.
    // If event is unique (i.e. does an event with same description at input date) adds event and returns true
    // otherwise returns false.
    template<class T> bool Calendar<T>::add_event(string desc, int day, int month, int year)
    {
        T date;
        try
        {
            date.set_date(year,month,day);
        }
        catch(std::exception &e)
        {
            return false;
        }

        for(size_t i = 0; i < events_.size(); i++)
        {
            // Make sure that we do not have the event on the same date already
            if(events_[i].second == desc && events_[i].first == date)
            {
                return false;
            }
        }
		
        events_.push_back(pair<T,string>(date,desc));

        // Sort events after date (ascending order)
        std::sort(events_.begin(), events_.end());
        return true;
    }

    // Sets the currentDate_ to input.
    // If date is invalid, returns false otherwise returns true.
    template<class T> bool Calendar<T>::set_date(int year, int month, int day)
    {
        T temp;
        try
        {
            temp.set_date(year,month,day);
        }
        catch(std::exception &e)
        {
            return false;
        }
        (*currentDate_) = temp;
        return true;
    }

    // Tries to remove an event at currentDate_.
    // If successful, returns true otherwise false.
    template<class T> bool Calendar<T>::remove_event(string desc)
    {
        return remove_event(desc,currentDate_->day(),currentDate_->month(),currentDate_->year());
    }

    // Tries to remove an event matching input. Fills in month & year from currentDate_.
    // If successful, returns true otherwise false.
    template<class T> bool Calendar<T>::remove_event(string desc, int day)
    {
        return remove_event(desc,day,currentDate_->month(),currentDate_->year());
    }

    // Tries to remove an event matching input. Fills in year from currentDate_.
    // If successful, returns true otherwise false.
    template<class T> bool Calendar<T>::remove_event(string desc, int day, int month)
    {
        return remove_event(desc,day,month,currentDate_->year());
    }

    // Tries to remove an event matching input.
    // If successful, returns true otherwise false.
    template<class T> bool Calendar<T>::remove_event(string desc, int day, int month, int year)
    {
        // Find event with matching date and description & remove it
        for(size_t i = 0; i < events_.size(); i++)
        {
            if(events_[i].first.year() == year && events_[i].second == desc && events_[i].first.month() == month && events_[i].first.day() == day)
            {
                events_.erase(events_.begin()+i);
                return true;
            }
        }
        return false;
    }
	
    // Assigns the calendar to input calendar of same type.
    template<class T> Calendar<T> & Calendar<T>::operator=(const Calendar<T> &calendar)
    {
        return assignment_helper(calendar, typename std::is_base_of<Date,T>::type());
    }
	
    // Assigns the calendar to input calendar.
    // Converts dates of events to corresponding calendar type.
	template<class T>
    template<class G>
    Calendar<T> & Calendar<T>::operator=(const Calendar<G> &calendar)
    {
        return assignment_helper(calendar, typename std::is_base_of<Date,G>::type()); // Make sure that G is derived from Date (will otherwise generate a compile error)
    }
	
    // Helper function to assignment.
	template<class T>
    template<class G>
	Calendar<T> & Calendar<T>::assignment_helper(const Calendar<G> &calendar, std::true_type)
	{
        *currentDate_ = *(calendar.currentDate_);
	
        events_.clear();
        for(size_t i = 0; i < calendar.events_.size(); i++)
        {
            events_.push_back(pair<T,string>(calendar.events_[i].first,calendar.events_[i].second));
        }
        return *this;
	}
}

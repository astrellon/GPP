#pragma once

#include <string>
#include <map>

template <class T>
int getTypeId() {
	return __COUNTER__;
}

template <>
int getTypeId<int>() {
	return __COUNTER__;
}

template <>
int getTypeId<float>() {
	return __COUNTER__;
}

template <>
int getTypeId<std::string>() {
	return __COUNTER__;
}

template <>
int getTypeId<std::map<std::string, int> >() {
	return __COUNTER__;
}

// Simplifed boost any type to work without real time type information any only
// uses unsafe casting.

// See http://www.boost.org/libs/any for Documentation.

#ifndef BOOST_ANY_INCLUDED
#define BOOST_ANY_INCLUDED

// what:  variant type boost::any
// who:   contributed by Kevlin Henney,
//        with features contributed and bugs found by
//        Ed Brey, Mark Rodgers, Peter Dimov, and James Curran
// when:  July 2001
// where: tested with BCC 5.5, MSVC 6.0, and g++ 2.95

//#include <algorithm>
//#include <typeinfo>

// See boost/python/type_id.hpp
// TODO: add BOOST_TYPEID_COMPARE_BY_NAME to config.hpp
# if (defined(__GNUC__) && __GNUC__ >= 3) \
 || defined(_AIX) \
 || (   defined(__sgi) && defined(__host_mips)) \
 || (defined(__hpux) && defined(__HP_aCC)) \
 || (defined(linux) && defined(__INTEL_COMPILER) && defined(__ICC))
#  define BOOST_AUX_ANY_TYPE_ID_NAME
#include <cstring>
# endif 

namespace boost
{
    class any
    {
    public: // structors

        any()
          : content(0)
        {
        }

        template<typename ValueType>
        any(const ValueType & value)
          : content(new holder<ValueType>(value))
        {
        }

        any(const any & other)
          : content(other.content ? other.content->clone() : 0)
        {
        }

        ~any()
        {
            delete content;
        }

    public: // modifiers

        any & swap(any & rhs)
        {
            std::swap(content, rhs.content);
            return *this;
        }

        template<typename ValueType>
        any & operator=(const ValueType & rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

        any & operator=(any rhs)
        {
            rhs.swap(*this);
            return *this;
        }

		template<typename ValueType>
		bool isType() const
		{
			return getTypeId<ValueType>() == content->type();
		}

    public: // queries

        bool empty() const
        {
            return !content;
        }

		int type() const
		{
			return content ? content->type() : getTypeId<void>();
		}
        
    private: // types

        class placeholder
        {
        public: // structors

            virtual ~placeholder()
            {
            }

        public: // queries

			virtual int type() const = 0;

            virtual placeholder * clone() const = 0;

        };

        template<typename ValueType>
        class holder : public placeholder
        {
        public: // structors

            holder(const ValueType & value)
              : held(value)
            {
            }

        public: // queries

			virtual int type() const
			{
				return getTypeId<ValueType>();
			}

            virtual placeholder * clone() const
            {
                return new holder(held);
            }

        public: // representation

            ValueType held;

        private: // intentionally left unimplemented
            holder & operator=(const holder &);
        };

    private: // representation

        template<typename ValueType>
        friend ValueType * any_cast(any *);

		template<typename ValueType>
        friend ValueType any_cast(any &);
		
	public:
        placeholder * content;

    };

    template<typename ValueType>
    inline ValueType * any_cast(any * operand)
    {
		return &static_cast<any::holder<ValueType> *>(operand->content)->held;
    }

    template<typename ValueType>
    inline const ValueType * any_cast(const any * operand)
    {
        return any_cast<ValueType>(const_cast<any *>(operand));
	}

	template <typename ValueType>
	inline ValueType any_cast(any & operand)
	{
		return static_cast<any::holder<ValueType> *>(operand.content)->held;
	}

}

// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#endif

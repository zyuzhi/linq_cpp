#pragma once
#include <list>

namespace zlinq
{
template <typename T>
struct function_traits
    : public function_traits<decltype(&T::operator())>
{};

template <typename ClassType, typename ReturnType, typename Arg1>
struct function_traits<ReturnType(ClassType::*)(Arg1) const>
{
    typedef ReturnType result_type;
};

template <typename T, template<typename E, typename Allocator=std::allocator<E> > class _Container=std::list>
class LinqRange
{
public:
	typedef T ElementType;
	typedef _Container<ElementType> SeqType;
	typedef LinqRange<ElementType> SelfType;
	typedef LinqRange<ElementType>& SelfReferenceType;
private:
	SeqType data;
public:
	template<typename Container>
	explicit LinqRange(Container &v)
		: data(std::begin(v), std::end(v))
	{
	}

	template<typename Iterator>
	LinqRange(Iterator itStart, Iterator itEnd)
		: data(itStart, itEnd)
	{
	}

	typename SeqType::iterator end()
	{
		return data.end();
	}

	typename SeqType::const_iterator end() const
	{
		return data.end();
	}

	typename SeqType::iterator begin()
	{
		return data.begin();
	}

	typename SeqType::const_iterator begin() const
	{
		return data.begin();
	}
	
	template<typename Function>
	SelfType Where(Function func)
	{
		SeqType res;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			if (func(*it))
			{
				res.push_back(*it);
			}
		}
		return SelfType(res);
	}

	template<typename Function>
	LinqRange<typename function_traits<Function>::result_type> Select(Function func)
	{
		typedef typename function_traits<Function>::result_type result_type;
		_Container<result_type> res;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			res.push_back(func(*it));
		}
		return LinqRange<result_type>(res);
	}

	template<typename Function>
	bool All(Function func)
	{
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			if (!func(*it))
			{
				return false;
			}
		}
		return true;
	}

	template<typename Function>
	bool Any(Function func)
	{
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			if (func(*it))
			{
				return true;
			}
		}
		return false;
	}

	SeqType ToList()
	{
		return data;
	}
};

class Linq
{
public:
	template<typename Container>
	static LinqRange<typename Container::value_type> From(Container& v)
	{
		return LinqRange<typename Container::value_type>(v);
	}

	template<typename Iterator>
	static LinqRange<typename std::iterator_traits<Iterator>::value_type> From(Iterator itStart, Iterator itEnd)
	{
		typedef LinqRange<typename std::iterator_traits<Iterator>::value_type> RetType;
		return RetType(itStart, itEnd);
	}
};
}

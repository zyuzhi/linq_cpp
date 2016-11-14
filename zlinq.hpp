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
class Range
{
public:
	typedef T ElementType;
	typedef _Container<ElementType> SeqType;
	typedef Range<ElementType> SelfType;
	typedef Range<ElementType>& SelfReferenceType;
private:
	SeqType data;
public:
	template<typename Container>
	explicit Range(Container &v)
		: data(std::begin(v), std::end(v))
	{
	}

	template<typename Iterator>
	Range(Iterator itStart, Iterator itEnd)
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
	SelfType Where(const Function& func) const
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
	Range<typename function_traits<Function>::result_type> Select(const Function& func) const
	{
		typedef typename function_traits<Function>::result_type result_type;
		_Container<result_type> res;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			res.push_back(func(*it));
		}
		return Range<result_type>(res);
	}

	template<typename Function>
	bool All(const Function& func) const
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
	bool Any(const Function& func) const
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

	template<typename Function>
	size_t Count(const Function& func) const
	{
		size_t count = 0;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			if (func(*it))
			{
				count++;
			}
		}
		return count;
	}

	size_t Count()
	{
		return data.size();
	}

	ElementType Sum()
	{
		ElementType sum = 0;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			sum += *it;
		}
		return sum;
	}

	template<typename Function>
	ElementType Sum(const Function& func)
	{
		ElementType sum;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			sum = func(sum, *it);
		}
		return sum;
	}

	ElementType Average()
	{
		auto size = Count();
		if (size == 0)
		{
			return 0;
		}
		return Sum() / size;
	}

	template<typename Function>
	ElementType Average(const Function& func)
	{
		auto size = Count();
		if (size == 0)
		{
			return 0;
		}
		return Sum(func) / size;
	}

	SeqType ToList()
	{
		return data;
	}
};

template<typename Container>
Range<typename Container::value_type> From(const Container& v)
{
	return Range<typename Container::value_type>(v);
}

template<typename Iterator>
Range<typename std::iterator_traits<Iterator>::value_type> From(const Iterator& itStart, const Iterator& itEnd)
{
	typedef Range<typename std::iterator_traits<Iterator>::value_type> RetType;
	return RetType(itStart, itEnd);
}

template<typename Type, size_t Size>
Range<Type> From(Type (&arr)[Size])
{
	return Range<Type>(arr, arr + Size);
}
}

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
class range
{
public:
	typedef T ElementType;
	typedef _Container<ElementType> SeqType;
	typedef range<ElementType> SelfType;
	typedef range<ElementType>& SelfReferenceType;
private:
	SeqType data;
public:
	template<typename Container>
	explicit range(Container &v)
		: data(std::begin(v), std::end(v))
	{
	}

	template<typename Iterator>
	range(Iterator itStart, Iterator itEnd)
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
	SelfType where(const Function& func) const
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
	range<typename function_traits<Function>::result_type> select(const Function& func) const
	{
		typedef typename function_traits<Function>::result_type result_type;
		_Container<result_type> res;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			res.push_back(func(*it));
		}
		return range<result_type>(res);
	}

	template<typename Function>
	bool all(const Function& func) const
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
	bool any(const Function& func) const
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
	size_t count(const Function& func) const
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

	size_t count()
	{
		return data.size();
	}

	ElementType sum()
	{
		ElementType sum = 0;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			sum += *it;
		}
		return sum;
	}

	template<typename Function>
	ElementType sum(const Function& func)
	{
		ElementType sum;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			sum = func(sum, *it);
		}
		return sum;
	}

	ElementType average()
	{
		auto size = count();
		if (size == 0)
		{
			return 0;
		}
		return sum() / size;
	}

	template<typename Function>
	ElementType Average(const Function& func)
	{
		auto size = count();
		if (size == 0)
		{
			return 0;
		}
		return sum(func) / size;
	}

	SeqType to_list()
	{
		return data;
	}
};

template<typename Container>
range<typename Container::value_type> from(const Container& v)
{
	return range<typename Container::value_type>(v);
}

template<typename Iterator>
range<typename std::iterator_traits<Iterator>::value_type> from(const Iterator& itStart, const Iterator& itEnd)
{
	typedef range<typename std::iterator_traits<Iterator>::value_type> RetType;
	return RetType(itStart, itEnd);
}

template<typename Type, size_t Size>
range<Type> from(Type (&arr)[Size])
{
	return range<Type>(arr, arr + Size);
}
}

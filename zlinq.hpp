#pragma once
#include <list>
#include <set>

namespace zlinq
{

class zlinq_exception
{
public:
	std::string message;
	zlinq_exception(const std::string& message)
		: message(message)
	{
	}
};
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
	range()
	{}

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

	size_t count() const
	{
		return data.size();
	}

	ElementType sum() const
	{
		ElementType sum = 0;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			sum += *it;
		}
		return sum;
	}

	template<typename Function>
	ElementType sum(const Function& func) const
	{
		ElementType sum;
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			sum = func(sum, *it);
		}
		return sum;
	}

	ElementType average() const
	{
		auto size = count();
		if (size == 0)
		{
			return 0;
		}
		return sum() / size;
	}

	template<typename Function>
	ElementType average(const Function& func) const
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

	bool contains(const ElementType& t) const
	{
		for (auto it = begin(); it != end(); ++it)
		{
			if (t == *it) return true;
		}
		return false;
	}

	bool empty() const
	{
		return begin() == end();
	}

	ElementType first() const
	{
		if (empty()) throw zlinq_exception("Failed to get a value from an empty collection.");
		return *begin();
	}

	ElementType first_or_default(const ElementType& value) const
	{
		return empty() ? value : *begin();
	}

	ElementType last() const
	{
		if (empty()) throw zlinq_exception("Failed to get a value from an empty collection.");
		auto it = end();
		--it;
		return *it;
	}

	ElementType last_or_default(const ElementType& value) const
	{
		if (empty())
		{
			return value;
		}
		auto it = end();
		--it;
		return *it;
	}

	range<ElementType> distinct() const
	{
		std::set<ElementType> set;
		std::list<ElementType> list;

		for (auto it = begin(); it != end(); ++it)
		{
			if (set.find(*it) == set.end())
			{
				set.insert(*it);
				list.push_back(*it);
			}
		}
		return range(list);
	}

	range<ElementType> except_with(const range<ElementType>& val) const
	{
		std::set<ElementType> set(val.begin(), val.end());
		std::list<ElementType> list;
		for (auto it = begin(); it != end(); ++it)
		{
			if (set.find(*it) == set.end())
			{
				list.push_back(*it);
			}
		}
		return range(list);
	}

	range<ElementType> intersect_with(const range<ElementType>& val) const
	{
		std::set<ElementType> set(val.begin(), val.end());
		std::list<ElementType> list;
		for (auto it = begin(); it != end(); ++it)
		{
			if (set.find(*it) != set.end())
			{
				list.push_back(*it);
			}
		}
		return range(list);
	}

	range<ElementType> concat(const range<ElementType>& val) const
	{
		std::list<ElementType> list(begin(), end());
		for (auto it = val.begin(); it != val.end(); ++it)
		{
			list.push_back(*it);
		}
		return range(list);
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

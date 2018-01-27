#ifndef NLOHMANN_JSON_DETAIL_ITERATORS_INTERNAL_ITERATOR_HPP
#define NLOHMANN_JSON_DETAIL_ITERATORS_INTERNAL_ITERATOR_HPP

#include "detail/iterators/primitive_iterator.hpp"

namespace nlohmann
{
	namespace detail
	{
		template<typename BasicJsonType> struct internal_iterator
		{
			typename BasicJsonType::object_t::iterator object_iterator{};
			typename BasicJsonType::array_t::iterator array_iterator{};
		};
	}
}

#endif

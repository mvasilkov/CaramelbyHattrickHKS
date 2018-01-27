#ifndef NLOHMANN_JSON_DETAIL_VALUE_T_HPP
#define NLOHMANN_JSON_DETAIL_VALUE_T_HPP

#include <ciso646>

namespace nlohmann
{
	namespace detail
	{
		enum class value_t : std::uint8_t
		{
			null,
			object,
			array,
			string,
			boolean,
			number_integer,
			number_unsigned,
			number_float,
		};
	}
}

#endif

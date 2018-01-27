#ifndef NLOHMANN_JSON_ADL_SERIALIZER_HPP
#define NLOHMANN_JSON_ADL_SERIALIZER_HPP

#include "detail/conversions/from_json.hpp"
#include "detail/conversions/to_json.hpp"

namespace nlohmann
{
	template <typename, typename> struct adl_serializer
	{
		template<typename BasicJsonType, typename ValueType>
		static void from_json(BasicJsonType&& j, ValueType& val) noexcept(
			noexcept(::nlohmann::from_json(std::forward<BasicJsonType>(j), val)))
		{
			::nlohmann::from_json(std::forward<BasicJsonType>(j), val);
		}
		template<typename BasicJsonType, typename ValueType>
		static void to_json(BasicJsonType& j, ValueType&& val) noexcept(
			noexcept(::nlohmann::to_json(j, std::forward<ValueType>(val))))
		{
			::nlohmann::to_json(j, std::forward<ValueType>(val));
		}
	};
}

#endif

#ifndef NLOHMANN_JSON_DETAIL_CONVERSIONS_FROM_JSON_HPP
#define NLOHMANN_JSON_DETAIL_CONVERSIONS_FROM_JSON_HPP

#include <ciso646>
#include <forward_list>
#include <string>
#include <valarray>

#include "detail/macro_scope.hpp"
#include "detail/meta.hpp"
#include "detail/value_t.hpp"

namespace nlohmann
{
	namespace detail
	{
		template<typename BasicJsonType, typename ArithmeticType,
			enable_if_t<std::is_arithmetic<ArithmeticType>::value and
			not std::is_same<ArithmeticType, typename BasicJsonType::boolean_t>::value,
			int> = 0>
			void get_arithmetic_value(const BasicJsonType& j, ArithmeticType& val)
		{
			switch (static_cast<value_t>(j))
			{
			case value_t::number_unsigned:
			{
				val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::number_unsigned_t*>());
				break;
			}
			case value_t::number_integer:
			{
				val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::number_integer_t*>());
				break;
			}
			case value_t::number_float:
			{
				val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::number_float_t*>());
				break;
			}
			}
		}

		template<typename BasicJsonType>
		void from_json(const BasicJsonType& j, typename BasicJsonType::boolean_t& b)
		{
			b = *j.template get_ptr<const typename BasicJsonType::boolean_t*>();
		}

		template<typename BasicJsonType>
		void from_json(const BasicJsonType& j, typename BasicJsonType::string_t& s)
		{
			s = *j.template get_ptr<const typename BasicJsonType::string_t*>();
		}

		template<typename BasicJsonType>
		void from_json(const BasicJsonType& j, typename BasicJsonType::number_float_t& val)
		{
			get_arithmetic_value(j, val);
		}

		template<typename BasicJsonType>
		void from_json(const BasicJsonType& j, typename BasicJsonType::number_unsigned_t& val)
		{
			get_arithmetic_value(j, val);
		}

		template<typename BasicJsonType>
		void from_json(const BasicJsonType& j, typename BasicJsonType::number_integer_t& val)
		{
			get_arithmetic_value(j, val);
		}

		template<typename BasicJsonType, typename EnumType,
			enable_if_t<std::is_enum<EnumType>::value, int> = 0>
			void from_json(const BasicJsonType& j, EnumType& e)
		{
			typename std::underlying_type<EnumType>::type val;
			get_arithmetic_value(j, val);
			e = static_cast<EnumType>(val);
		}

		template<typename BasicJsonType>
		void from_json(const BasicJsonType& j, typename BasicJsonType::array_t& arr)
		{
			arr = *j.template get_ptr<const typename BasicJsonType::array_t*>();
		}

		template<typename BasicJsonType, typename T, typename Allocator,
			enable_if_t<std::is_convertible<BasicJsonType, T>::value, int> = 0>
			void from_json(const BasicJsonType& j, std::forward_list<T, Allocator>& l)
		{
			std::transform(j.rbegin(), j.rend(),
				std::front_inserter(l), [](const BasicJsonType & i)
			{
				return i.template get<T>();
			});
		}

		template<typename BasicJsonType, typename T,
			enable_if_t<std::is_convertible<BasicJsonType, T>::value, int> = 0>
			void from_json(const BasicJsonType& j, std::valarray<T>& l)
		{
			l.resize(j.size());
			std::copy(j.m_value.array->begin(), j.m_value.array->end(), std::begin(l));
		}

		template<typename BasicJsonType, typename CompatibleArrayType>
		void from_json_array_impl(const BasicJsonType& j, CompatibleArrayType& arr, priority_tag<0>)
		{
			using std::end;

			std::transform(j.begin(), j.end(),
				std::inserter(arr, end(arr)), [](const BasicJsonType & i)
			{
				return i.template get<typename CompatibleArrayType::value_type>();
			});
		}

		template<typename BasicJsonType, typename CompatibleArrayType>
		auto from_json_array_impl(const BasicJsonType& j, CompatibleArrayType& arr, priority_tag<1>)
			-> decltype(
				arr.reserve(std::declval<typename CompatibleArrayType::size_type>()),
				void())
		{
			using std::end;

			arr.reserve(j.size());
			std::transform(j.begin(), j.end(),
				std::inserter(arr, end(arr)), [](const BasicJsonType & i)
			{
				return i.template get<typename CompatibleArrayType::value_type>();
			});
		}

		template<typename BasicJsonType, typename T, std::size_t N>
		void from_json_array_impl(const BasicJsonType& j, std::array<T, N>& arr, priority_tag<2>)
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				arr[i] = j.at(i).template get<T>();
			}
		}

		template<typename BasicJsonType, typename CompatibleArrayType,
			enable_if_t<is_compatible_array_type<BasicJsonType, CompatibleArrayType>::value and
			std::is_convertible<BasicJsonType, typename CompatibleArrayType::value_type>::value and
			not std::is_same<typename BasicJsonType::array_t, CompatibleArrayType>::value, int> = 0>
			void from_json(const BasicJsonType& j, CompatibleArrayType& arr)
		{
			from_json_array_impl(j, arr, priority_tag<2> {});
		}

		template<typename BasicJsonType, typename CompatibleObjectType,
			enable_if_t<is_compatible_object_type<BasicJsonType, CompatibleObjectType>::value, int> = 0>
			void from_json(const BasicJsonType& j, CompatibleObjectType& obj)
		{
			auto inner_object = j.template get_ptr<const typename BasicJsonType::object_t*>();
			using value_type = typename CompatibleObjectType::value_type;
			std::transform(
				inner_object->begin(), inner_object->end(),
				std::inserter(obj, obj.begin()),
				[](typename BasicJsonType::object_t::value_type const & p)
			{
				return value_type(p.first, p.second.template get<typename CompatibleObjectType::mapped_type>());
			});
		}

		template<typename BasicJsonType, typename ArithmeticType,
			enable_if_t <
			std::is_arithmetic<ArithmeticType>::value and
			not std::is_same<ArithmeticType, typename BasicJsonType::number_unsigned_t>::value and
			not std::is_same<ArithmeticType, typename BasicJsonType::number_integer_t>::value and
			not std::is_same<ArithmeticType, typename BasicJsonType::number_float_t>::value and
			not std::is_same<ArithmeticType, typename BasicJsonType::boolean_t>::value,
			int> = 0>
			void from_json(const BasicJsonType& j, ArithmeticType& val)
		{
			switch (static_cast<value_t>(j))
			{
			case value_t::number_unsigned:
			{
				val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::number_unsigned_t*>());
				break;
			}
			case value_t::number_integer:
			{
				val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::number_integer_t*>());
				break;
			}
			case value_t::number_float:
			{
				val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::number_float_t*>());
				break;
			}
			case value_t::boolean:
			{
				val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::boolean_t*>());
				break;
			}
			}
		}

		template<typename BasicJsonType, typename A1, typename A2>
		void from_json(const BasicJsonType& j, std::pair<A1, A2>& p)
		{
			p = { j.at(0).template get<A1>(), j.at(1).template get<A2>() };
		}

		template<typename BasicJsonType, typename Tuple, std::size_t... Idx>
		void from_json_tuple_impl(const BasicJsonType& j, Tuple& t, index_sequence<Idx...>)
		{
			t = std::make_tuple(j.at(Idx).template get<typename std::tuple_element<Idx, Tuple>::type>()...);
		}

		template<typename BasicJsonType, typename... Args>
		void from_json(const BasicJsonType& j, std::tuple<Args...>& t)
		{
			from_json_tuple_impl(j, t, index_sequence_for<Args...> {});
		}

		struct from_json_fn
		{
		private:
			template<typename BasicJsonType, typename T>
			auto call(const BasicJsonType& j, T& val, priority_tag<1>) const
				noexcept(noexcept(from_json(j, val)))
				-> decltype(from_json(j, val), void())
			{
				return from_json(j, val);
			}

			template<typename BasicJsonType, typename T>
			void call(const BasicJsonType&, T&, priority_tag<0>) const noexcept
			{
			}

		public:
			template<typename BasicJsonType, typename T>
			void operator()(const BasicJsonType& j, T& val) const
				noexcept(noexcept(std::declval<from_json_fn>().call(j, val, priority_tag<1> {})))
			{
				return call(j, val, priority_tag<1> {});
			}
		};
	}

	namespace
	{
		constexpr const auto& from_json = detail::static_const<detail::from_json_fn>::value;
	}
}

#endif

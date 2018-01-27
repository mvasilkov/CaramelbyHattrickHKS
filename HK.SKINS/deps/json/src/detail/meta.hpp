#ifndef NLOHMANN_JSON_DETAIL_META_HPP
#define NLOHMANN_JSON_DETAIL_META_HPP

#include <ciso646>

#include "json_fwd.hpp"
#include "detail/macro_scope.hpp"

namespace nlohmann
{
	namespace detail
	{
		template<typename> struct is_basic_json : std::false_type {};

		NLOHMANN_BASIC_JSON_TPL_DECLARATION
			struct is_basic_json<NLOHMANN_BASIC_JSON_TPL> : std::true_type {};

		template<bool B, typename T = void>
		using enable_if_t = typename std::enable_if<B, T>::type;

		template<typename T>
		using uncvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

		template<std::size_t... Ints>
		struct index_sequence
		{
			using type = index_sequence;
			using value_type = std::size_t;
			static constexpr std::size_t size() noexcept
			{
				return sizeof...(Ints);
			}
		};

		template<class Sequence1, class Sequence2>
		struct merge_and_renumber;

		template<std::size_t... I1, std::size_t... I2>
		struct merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
			: index_sequence < I1..., (sizeof...(I1)+I2)... > {};

		template<std::size_t N>
		struct make_index_sequence
			: merge_and_renumber < typename make_index_sequence < N / 2 >::type,
			typename make_index_sequence < N - N / 2 >::type > {};

		template<> struct make_index_sequence<0> : index_sequence<> {};
		template<> struct make_index_sequence<1> : index_sequence<0> {};

		template<typename... Ts>
		using index_sequence_for = make_index_sequence<sizeof...(Ts)>;

		template<class...> struct conjunction : std::true_type {};
		template<class B1> struct conjunction<B1> : B1 {};
		template<class B1, class... Bn>
		struct conjunction<B1, Bn...> : std::conditional<bool(B1::value), conjunction<Bn...>, B1>::type {};
		template<class B> struct negation : std::integral_constant<bool, not B::value> {};
		template<unsigned N> struct priority_tag : priority_tag < N - 1 > {};
		template<> struct priority_tag<0> {};

		NLOHMANN_JSON_HAS_HELPER(mapped_type);
		NLOHMANN_JSON_HAS_HELPER(key_type);
		NLOHMANN_JSON_HAS_HELPER(value_type);
		NLOHMANN_JSON_HAS_HELPER(iterator);

		template<bool B, class RealType, class CompatibleObjectType>
		struct is_compatible_object_type_impl : std::false_type {};

		template<class RealType, class CompatibleObjectType>
		struct is_compatible_object_type_impl<true, RealType, CompatibleObjectType>
		{
			static constexpr auto value =
				std::is_constructible<typename RealType::key_type, typename CompatibleObjectType::key_type>::value and
				std::is_constructible<typename RealType::mapped_type, typename CompatibleObjectType::mapped_type>::value;
		};

		template<class BasicJsonType, class CompatibleObjectType>
		struct is_compatible_object_type
		{
			static auto constexpr value = is_compatible_object_type_impl <
				conjunction<negation<std::is_same<void, CompatibleObjectType>>,
				has_mapped_type<CompatibleObjectType>,
				has_key_type<CompatibleObjectType>>::value,
				typename BasicJsonType::object_t, CompatibleObjectType >::value;
		};

		template<typename BasicJsonType, typename T>
		struct is_basic_json_nested_type
		{
			static auto constexpr value = std::is_same<T, typename BasicJsonType::iterator>::value or
				std::is_same<T, typename BasicJsonType::const_iterator>::value or
				std::is_same<T, typename BasicJsonType::reverse_iterator>::value or
				std::is_same<T, typename BasicJsonType::const_reverse_iterator>::value;
		};

		template<class BasicJsonType, class CompatibleArrayType>
		struct is_compatible_array_type
		{
			static auto constexpr value =
				conjunction<negation<std::is_same<void, CompatibleArrayType>>,
				negation<is_compatible_object_type<
				BasicJsonType, CompatibleArrayType>>,
				negation<std::is_constructible<typename BasicJsonType::string_t,
				CompatibleArrayType>>,
				negation<is_basic_json_nested_type<BasicJsonType, CompatibleArrayType>>,
				has_value_type<CompatibleArrayType>,
				has_iterator<CompatibleArrayType>>::value;
		};

		template<bool, typename, typename>
		struct is_compatible_integer_type_impl : std::false_type {};

		template<typename RealIntegerType, typename CompatibleNumberIntegerType>
		struct is_compatible_integer_type_impl<true, RealIntegerType, CompatibleNumberIntegerType>
		{
			using RealLimits = std::numeric_limits<RealIntegerType>;
			using CompatibleLimits = std::numeric_limits<CompatibleNumberIntegerType>;

			static constexpr auto value =
				std::is_constructible<RealIntegerType, CompatibleNumberIntegerType>::value and
				CompatibleLimits::is_integer and
				RealLimits::is_signed == CompatibleLimits::is_signed;
		};

		template<typename RealIntegerType, typename CompatibleNumberIntegerType>
		struct is_compatible_integer_type
		{
			static constexpr auto value =
				is_compatible_integer_type_impl <
				std::is_integral<CompatibleNumberIntegerType>::value and
				not std::is_same<bool, CompatibleNumberIntegerType>::value,
				RealIntegerType, CompatibleNumberIntegerType > ::value;
		};

		template<typename BasicJsonType, typename T>
		struct has_from_json
		{
		private:
			template<typename U, typename = enable_if_t<std::is_same<void, decltype(uncvref_t<U>::from_json(
				std::declval<BasicJsonType>(), std::declval<T&>()))>::value>>
				static int detect(U&&);
			static void detect(...);

		public:
			static constexpr bool value = std::is_integral<decltype(
				detect(std::declval<typename BasicJsonType::template json_serializer<T, void>>()))>::value;
		};

		template<typename BasicJsonType, typename T>
		struct has_non_default_from_json
		{
		private:
			template <
				typename U,
				typename = enable_if_t<std::is_same<
				T, decltype(uncvref_t<U>::from_json(std::declval<BasicJsonType>()))>::value >>
				static int detect(U&&);
			static void detect(...);

		public:
			static constexpr bool value = std::is_integral<decltype(detect(
				std::declval<typename BasicJsonType::template json_serializer<T, void>>()))>::value;
		};

		template<typename BasicJsonType, typename T>
		struct has_to_json
		{
		private:
			template<typename U, typename = decltype(uncvref_t<U>::to_json(
				std::declval<BasicJsonType&>(), std::declval<T>()))>
				static int detect(U&&);
			static void detect(...);

		public:
			static constexpr bool value = std::is_integral<decltype(detect(
				std::declval<typename BasicJsonType::template json_serializer<T, void>>()))>::value;
		};

		template<typename T>
		struct static_const
		{
			static constexpr T value{};
		};

		template<typename T>
		constexpr T static_const<T>::value;
	}
}

#endif


#ifndef NLOHMANN_JSON_HPP
#define NLOHMANN_JSON_HPP

#include <ciso646>
#include <string>

#include "json_fwd.hpp"
#include "detail/macro_scope.hpp"
#include "detail/meta.hpp"
#include "detail/value_t.hpp"
#include "detail/conversions/from_json.hpp"
#include "detail/conversions/to_json.hpp"
#include "detail/parsing/input_adapters.hpp"
#include "detail/parsing/lexer.hpp"
#include "detail/parsing/parser.hpp"
#include "detail/iterators/primitive_iterator.hpp"
#include "detail/iterators/internal_iterator.hpp"
#include "detail/iterators/iter_impl.hpp"
#include "detail/iterators/iteration_proxy.hpp"
#include "detail/iterators/json_reverse_iterator.hpp"
#include "detail/parsing/output_adapters.hpp"
#include "detail/serializer.hpp"
#include "detail/json_ref.hpp"
#include "adl_serializer.hpp"

namespace nlohmann
{
	NLOHMANN_BASIC_JSON_TPL_DECLARATION
		class basic_json
	{
	private:
		template<detail::value_t> friend struct detail::external_constructor;
		friend ::nlohmann::detail::parser<basic_json>;
		friend ::nlohmann::detail::serializer<basic_json>;
		template<typename BasicJsonType>
		friend class ::nlohmann::detail::iter_impl;

		using basic_json_t = NLOHMANN_BASIC_JSON_TPL;

		using lexer = ::nlohmann::detail::lexer<basic_json>;
		using parser = ::nlohmann::detail::parser<basic_json>;

		template<typename BasicJsonType>
		using internal_iterator = ::nlohmann::detail::internal_iterator<BasicJsonType>;
		template<typename BasicJsonType>
		using iter_impl = ::nlohmann::detail::iter_impl<BasicJsonType>;
		template<typename Iterator>
		using iteration_proxy = ::nlohmann::detail::iteration_proxy<Iterator>;
		template<typename Base> using json_reverse_iterator = ::nlohmann::detail::json_reverse_iterator<Base>;

		template<typename CharType>
		using output_adapter_t = ::nlohmann::detail::output_adapter_t<CharType>;

		using serializer = ::nlohmann::detail::serializer<basic_json>;

	public:
		using value_t = detail::value_t;
		template<typename T, typename SFINAE>
		using json_serializer = JSONSerializer<T, SFINAE>;
		using initializer_list_t = std::initializer_list<detail::json_ref<basic_json>>;
		using value_type = basic_json;
		using reference = value_type & ;
		using const_reference = const value_type&;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
		using allocator_type = AllocatorType<basic_json>;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
		using iterator = iter_impl<basic_json>;
		using const_iterator = iter_impl<const basic_json>;
		using reverse_iterator = json_reverse_iterator<typename basic_json::iterator>;
		using const_reverse_iterator = json_reverse_iterator<typename basic_json::const_iterator>;

		static allocator_type get_allocator()
		{
			return allocator_type();
		}

		using object_comparator_t = std::less<>;
		using object_t = ObjectType<StringType,
			basic_json,
			object_comparator_t,
			AllocatorType<std::pair<const StringType,
			basic_json>>>;

		using array_t = ArrayType<basic_json, AllocatorType<basic_json>>;
		using string_t = StringType;
		using boolean_t = BooleanType;
		using number_integer_t = NumberIntegerType;
		using number_unsigned_t = NumberUnsignedType;
		using number_float_t = NumberFloatType;

	private:

		template<typename T, typename... Args>
		static T* create(Args&& ... args)
		{
			AllocatorType<T> alloc;
			using AllocatorTraits = std::allocator_traits<AllocatorType<T>>;

			auto deleter = [&](T * object)
			{
				AllocatorTraits::deallocate(alloc, object, 1);
			};
			std::unique_ptr<T, decltype(deleter)> object(AllocatorTraits::allocate(alloc, 1), deleter);
			AllocatorTraits::construct(alloc, object.get(), std::forward<Args>(args)...);
			return object.release();
		}

		union json_value
		{
			object_t* object;
			array_t* array;
			string_t* string;
			boolean_t boolean;
			number_integer_t number_integer;
			number_unsigned_t number_unsigned;
			number_float_t number_float;

			json_value() = default;
			json_value(boolean_t v) noexcept : boolean(v) {}
			json_value(number_integer_t v) noexcept : number_integer(v) {}
			json_value(number_unsigned_t v) noexcept : number_unsigned(v) {}
			json_value(number_float_t v) noexcept : number_float(v) {}
			json_value(value_t t)
			{
				switch (t)
				{
				case value_t::object:
				{
					object = create<object_t>();
					break;
				}

				case value_t::array:
				{
					array = create<array_t>();
					break;
				}

				case value_t::string:
				{
					string = create<string_t>();
					break;
				}

				case value_t::boolean:
				{
					boolean = boolean_t(false);
					break;
				}

				case value_t::number_integer:
				{
					number_integer = number_integer_t(0);
					break;
				}

				case value_t::number_unsigned:
				{
					number_unsigned = number_unsigned_t(0);
					break;
				}

				case value_t::number_float:
				{
					number_float = number_float_t(0.0);
					break;
				}

				case value_t::null:
				{
					object = nullptr;
					break;
				}

				default:
				{
					object = nullptr;
					break;
				}
				}
			}

			json_value(const string_t& value)
			{
				string = create<string_t>(value);
			}

			json_value(string_t&& value)
			{
				string = create<string_t>(std::move(value));
			}

			json_value(const object_t& value)
			{
				object = create<object_t>(value);
			}

			json_value(object_t&& value)
			{
				object = create<object_t>(std::move(value));
			}

			json_value(const array_t& value)
			{
				array = create<array_t>(value);
			}

			json_value(array_t&& value)
			{
				array = create<array_t>(std::move(value));
			}

			void destroy(value_t t)
			{
				switch (t)
				{
				case value_t::object:
				{
					AllocatorType<object_t> alloc;
					std::allocator_traits<decltype(alloc)>::destroy(alloc, object);
					std::allocator_traits<decltype(alloc)>::deallocate(alloc, object, 1);
					break;
				}

				case value_t::array:
				{
					AllocatorType<array_t> alloc;
					std::allocator_traits<decltype(alloc)>::destroy(alloc, array);
					std::allocator_traits<decltype(alloc)>::deallocate(alloc, array, 1);
					break;
				}

				case value_t::string:
				{
					AllocatorType<string_t> alloc;
					std::allocator_traits<decltype(alloc)>::destroy(alloc, string);
					std::allocator_traits<decltype(alloc)>::deallocate(alloc, string, 1);
					break;
				}
				}
			}
		};

	public:

		using parse_event_t = typename parser::parse_event_t;
		using parser_callback_t = typename parser::parser_callback_t;

		basic_json(const value_t v)
			: m_type(v), m_value(v)
		{
		}

		basic_json(std::nullptr_t = nullptr) noexcept
			: basic_json(value_t::null)
		{
		}

		template<typename CompatibleType, typename U = detail::uncvref_t<CompatibleType>,
			detail::enable_if_t<not std::is_base_of<std::istream, U>::value and
			not std::is_same<U, basic_json_t>::value and
			not detail::is_basic_json_nested_type<
			basic_json_t, U>::value and
			detail::has_to_json<basic_json, U>::value,
			int> = 0>
			basic_json(CompatibleType && val) noexcept(noexcept(JSONSerializer<U>::to_json(
				std::declval<basic_json_t&>(), std::forward<CompatibleType>(val))))
		{
			JSONSerializer<U>::to_json(*this, std::forward<CompatibleType>(val));
		}

		basic_json(initializer_list_t init,
			bool type_deduction = true,
			value_t manual_type = value_t::array)
		{
			bool is_an_object = std::all_of(init.begin(), init.end(),
				[](const detail::json_ref<basic_json>& element_ref)
			{
				return (element_ref->is_array() and element_ref->size() == 2 and (*element_ref)[0].is_string());
			});

			if (not type_deduction)
			{
				if (manual_type == value_t::array)
				{
					is_an_object = false;
				}
			}

			if (is_an_object)
			{
				m_type = value_t::object;
				m_value = value_t::object;

				std::for_each(init.begin(), init.end(), [this](const detail::json_ref<basic_json>& element_ref)
				{
					auto element = element_ref.moved_or_copied();
					m_value.object->emplace(
						std::move(*((*element.m_value.array)[0].m_value.string)),
						std::move((*element.m_value.array)[1]));
				});
			}
			else
			{
				m_type = value_t::array;
				m_value.array = create<array_t>(init.begin(), init.end());
			}
		}

		static basic_json array(initializer_list_t init = {})
		{
			return basic_json(init, false, value_t::array);
		}

		static basic_json object(initializer_list_t init = {})
		{
			return basic_json(init, false, value_t::object);
		}

		basic_json(size_type cnt, const basic_json& val)
			: m_type(value_t::array)
		{
			m_value.array = create<array_t>(cnt, val);
		}

		template<class InputIT, typename std::enable_if<
			std::is_same<InputIT, typename basic_json_t::iterator>::value or
			std::is_same<InputIT, typename basic_json_t::const_iterator>::value, int>::type = 0>
			basic_json(InputIT first, InputIT last)
		{
			m_type = first.m_object->m_type;

			switch (m_type)
			{
			case value_t::number_integer:
			{
				m_value.number_integer = first.m_object->m_value.number_integer;
				break;
			}

			case value_t::number_unsigned:
			{
				m_value.number_unsigned = first.m_object->m_value.number_unsigned;
				break;
			}

			case value_t::number_float:
			{
				m_value.number_float = first.m_object->m_value.number_float;
				break;
			}

			case value_t::boolean:
			{
				m_value.boolean = first.m_object->m_value.boolean;
				break;
			}

			case value_t::string:
			{
				m_value = *first.m_object->m_value.string;
				break;
			}

			case value_t::object:
			{
				m_value.object = create<object_t>(first.m_it.object_iterator,
					last.m_it.object_iterator);
				break;
			}

			case value_t::array:
			{
				m_value.array = create<array_t>(first.m_it.array_iterator,
					last.m_it.array_iterator);
				break;
			}
			}
		}

		basic_json(const detail::json_ref<basic_json>& ref)
			: basic_json(ref.moved_or_copied())
		{}

		basic_json(const basic_json& other)
			: m_type(other.m_type)
		{
			switch (m_type)
			{
			case value_t::object:
			{
				m_value = *other.m_value.object;
				break;
			}

			case value_t::array:
			{
				m_value = *other.m_value.array;
				break;
			}

			case value_t::string:
			{
				m_value = *other.m_value.string;
				break;
			}

			case value_t::boolean:
			{
				m_value = other.m_value.boolean;
				break;
			}

			case value_t::number_integer:
			{
				m_value = other.m_value.number_integer;
				break;
			}

			case value_t::number_unsigned:
			{
				m_value = other.m_value.number_unsigned;
				break;
			}

			case value_t::number_float:
			{
				m_value = other.m_value.number_float;
				break;
			}
			}
		}

		basic_json(basic_json&& other) noexcept
			: m_type(std::move(other.m_type)),
			m_value(std::move(other.m_value))
		{
			other.m_type = value_t::null;
			other.m_value = {};
		}

		reference& operator=(basic_json other) noexcept (
			std::is_nothrow_move_constructible<value_t>::value and
			std::is_nothrow_move_assignable<value_t>::value and
			std::is_nothrow_move_constructible<json_value>::value and
			std::is_nothrow_move_assignable<json_value>::value
			)
		{
			using std::swap;
			swap(m_type, other.m_type);
			swap(m_value, other.m_value);

			return *this;
		}

		~basic_json()
		{
			m_value.destroy(m_type);
		}

	public:

		string_t dump(const int indent = -1, const char indent_char = ' ',
			const bool ensure_ascii = false) const
		{
			string_t result;
			serializer s(detail::output_adapter<char>(result), indent_char);

			if (indent >= 0)
			{
				s.dump(*this, true, ensure_ascii, static_cast<unsigned int>(indent));
			}
			else
			{
				s.dump(*this, false, ensure_ascii, 0);
			}

			return result;
		}

		constexpr value_t type() const noexcept
		{
			return m_type;
		}

		constexpr bool is_primitive() const noexcept
		{
			return is_null() or is_string() or is_boolean() or is_number();
		}

		constexpr bool is_structured() const noexcept
		{
			return is_array() or is_object();
		}

		constexpr bool is_null() const noexcept
		{
			return (m_type == value_t::null);
		}

		constexpr bool is_boolean() const noexcept
		{
			return (m_type == value_t::boolean);
		}

		constexpr bool is_number() const noexcept
		{
			return is_number_integer() or is_number_float();
		}

		constexpr bool is_number_integer() const noexcept
		{
			return (m_type == value_t::number_integer or m_type == value_t::number_unsigned);
		}

		constexpr bool is_number_unsigned() const noexcept
		{
			return (m_type == value_t::number_unsigned);
		}

		constexpr bool is_number_float() const noexcept
		{
			return (m_type == value_t::number_float);
		}

		constexpr bool is_object() const noexcept
		{
			return (m_type == value_t::object);
		}

		constexpr bool is_array() const noexcept
		{
			return (m_type == value_t::array);
		}

		constexpr bool is_string() const noexcept
		{
			return (m_type == value_t::string);
		}

		constexpr operator value_t() const noexcept
		{
			return m_type;
		}

	private:

		boolean_t get_impl(boolean_t*) const
		{
			if (JSON_LIKELY(is_boolean()))
			{
				return m_value.boolean;
			}
		}

		object_t* get_impl_ptr(object_t*) noexcept
		{
			return is_object() ? m_value.object : nullptr;
		}

		constexpr const object_t* get_impl_ptr(const object_t*) const noexcept
		{
			return is_object() ? m_value.object : nullptr;
		}

		array_t* get_impl_ptr(array_t*) noexcept
		{
			return is_array() ? m_value.array : nullptr;
		}

		constexpr const array_t* get_impl_ptr(const array_t*) const noexcept
		{
			return is_array() ? m_value.array : nullptr;
		}

		string_t* get_impl_ptr(string_t*) noexcept
		{
			return is_string() ? m_value.string : nullptr;
		}

		constexpr const string_t* get_impl_ptr(const string_t*) const noexcept
		{
			return is_string() ? m_value.string : nullptr;
		}

		boolean_t* get_impl_ptr(boolean_t*) noexcept
		{
			return is_boolean() ? &m_value.boolean : nullptr;
		}

		constexpr const boolean_t* get_impl_ptr(const boolean_t*) const noexcept
		{
			return is_boolean() ? &m_value.boolean : nullptr;
		}

		number_integer_t* get_impl_ptr(number_integer_t*) noexcept
		{
			return is_number_integer() ? &m_value.number_integer : nullptr;
		}

		constexpr const number_integer_t* get_impl_ptr(const number_integer_t*) const noexcept
		{
			return is_number_integer() ? &m_value.number_integer : nullptr;
		}

		number_unsigned_t* get_impl_ptr(number_unsigned_t*) noexcept
		{
			return is_number_unsigned() ? &m_value.number_unsigned : nullptr;
		}

		constexpr const number_unsigned_t* get_impl_ptr(const number_unsigned_t*) const noexcept
		{
			return is_number_unsigned() ? &m_value.number_unsigned : nullptr;
		}

		number_float_t* get_impl_ptr(number_float_t*) noexcept
		{
			return is_number_float() ? &m_value.number_float : nullptr;
		}

		constexpr const number_float_t* get_impl_ptr(const number_float_t*) const noexcept
		{
			return is_number_float() ? &m_value.number_float : nullptr;
		}

		template<typename ReferenceType, typename ThisType>
		static ReferenceType get_ref_impl(ThisType& obj)
		{
			auto ptr = obj.template get_ptr<typename std::add_pointer<ReferenceType>::type>();

			if (JSON_LIKELY(ptr != nullptr))
			{
				return *ptr;
			}
		}

	public:

		template<typename BasicJsonType, detail::enable_if_t<
			std::is_same<typename std::remove_const<BasicJsonType>::type, basic_json_t>::value,
			int> = 0>
			basic_json get() const
		{
			return *this;
		}

		template<typename ValueTypeCV, typename ValueType = detail::uncvref_t<ValueTypeCV>,
			detail::enable_if_t <
			not std::is_same<basic_json_t, ValueType>::value and
			detail::has_from_json<basic_json_t, ValueType>::value and
			not detail::has_non_default_from_json<basic_json_t, ValueType>::value,
			int> = 0>
			ValueType get() const noexcept(noexcept(
				JSONSerializer<ValueType>::from_json(std::declval<const basic_json_t&>(), std::declval<ValueType&>())))
		{
			ValueType ret;
			JSONSerializer<ValueType>::from_json(*this, ret);
			return ret;
		}

		template<typename ValueTypeCV, typename ValueType = detail::uncvref_t<ValueTypeCV>,
			detail::enable_if_t<not std::is_same<basic_json_t, ValueType>::value and
			detail::has_non_default_from_json<basic_json_t, ValueType>::value,
			int> = 0>
			ValueType get() const noexcept(noexcept(
				JSONSerializer<ValueTypeCV>::from_json(std::declval<const basic_json_t&>())))
		{
			return JSONSerializer<ValueTypeCV>::from_json(*this);
		}

		template<typename PointerType, typename std::enable_if<
			std::is_pointer<PointerType>::value, int>::type = 0>
			PointerType get() noexcept
		{
			return get_ptr<PointerType>();
		}

		template<typename PointerType, typename std::enable_if<
			std::is_pointer<PointerType>::value, int>::type = 0>
			constexpr const PointerType get() const noexcept
		{
			return get_ptr<PointerType>();
		}

		template<typename PointerType, typename std::enable_if<
			std::is_pointer<PointerType>::value, int>::type = 0>
			PointerType get_ptr() noexcept
		{
			using pointee_t = typename std::remove_const<typename
				std::remove_pointer<typename
				std::remove_const<PointerType>::type>::type>::type;

			return get_impl_ptr(static_cast<PointerType>(nullptr));
		}

		template<typename PointerType, typename std::enable_if<
			std::is_pointer<PointerType>::value and
			std::is_const<typename std::remove_pointer<PointerType>::type>::value, int>::type = 0>
			constexpr const PointerType get_ptr() const noexcept
		{
			using pointee_t = typename std::remove_const<typename
				std::remove_pointer<typename
				std::remove_const<PointerType>::type>::type>::type;

			return get_impl_ptr(static_cast<PointerType>(nullptr));
		}

		template<typename ReferenceType, typename std::enable_if<
			std::is_reference<ReferenceType>::value, int>::type = 0>
			ReferenceType get_ref()
		{
			return get_ref_impl<ReferenceType>(*this);
		}

		template<typename ReferenceType, typename std::enable_if<
			std::is_reference<ReferenceType>::value and
			std::is_const<typename std::remove_reference<ReferenceType>::type>::value, int>::type = 0>
			ReferenceType get_ref() const
		{
			return get_ref_impl<ReferenceType>(*this);
		}

		template < typename ValueType, typename std::enable_if <
			not std::is_pointer<ValueType>::value and
			not std::is_same<ValueType, detail::json_ref<basic_json>>::value and
			not std::is_same<ValueType, typename string_t::value_type>::value
			, int >::type = 0 >
			operator ValueType() const
		{
			return get<ValueType>();
		}

		reference at(size_type idx)
		{
			if (JSON_LIKELY(is_array()))
			{
				return m_value.array->at(idx);
			}
		}

		const_reference at(size_type idx) const
		{
			if (JSON_LIKELY(is_array()))
			{
				return m_value.array->at(idx);
			}
		}

		reference at(const typename object_t::key_type& key)
		{
			if (JSON_LIKELY(is_object()))
			{
				return m_value.object->at(key);
			}
		}

		const_reference at(const typename object_t::key_type& key) const
		{
			if (JSON_LIKELY(is_object()))
			{
				return m_value.object->at(key);
			}
		}

		reference operator[](size_type idx)
		{
			if (is_null())
			{
				m_type = value_t::array;
				m_value.array = create<array_t>();
			}

			if (JSON_LIKELY(is_array()))
			{
				if (idx >= m_value.array->size())
				{
					m_value.array->insert(m_value.array->end(),
						idx - m_value.array->size() + 1,
						basic_json());
				}

				return m_value.array->operator[](idx);
			}
		}

		const_reference operator[](size_type idx) const
		{
			if (JSON_LIKELY(is_array()))
			{
				return m_value.array->operator[](idx);
			}
		}

		reference operator[](const typename object_t::key_type& key)
		{
			if (is_null())
			{
				m_type = value_t::object;
				m_value.object = create<object_t>();
			}

			if (JSON_LIKELY(is_object()))
			{
				return m_value.object->operator[](key);
			}
		}

		const_reference operator[](const typename object_t::key_type& key) const
		{
			if (JSON_LIKELY(is_object()))
			{
				return m_value.object->find(key)->second;
			}
		}

		template<typename T>
		reference operator[](T* key)
		{
			if (is_null())
			{
				m_type = value_t::object;
				m_value = value_t::object;
			}

			if (JSON_LIKELY(is_object()))
			{
				return m_value.object->operator[](key);
			}
		}

		template<typename T>
		const_reference operator[](T* key) const
		{
			if (JSON_LIKELY(is_object()))
			{
				return m_value.object->find(key)->second;
			}
		}

		template<class ValueType, typename std::enable_if<
			std::is_convertible<basic_json_t, ValueType>::value, int>::type = 0>
			ValueType value(const typename object_t::key_type& key, const ValueType& default_value) const
		{
			if (JSON_LIKELY(is_object()))
			{
				const auto it = find(key);
				if (it != end())
				{
					return *it;
				}

				return default_value;
			}
		}

		string_t value(const typename object_t::key_type& key, const char* default_value) const
		{
			return value(key, string_t(default_value));
		}

		reference front()
		{
			return *begin();
		}

		const_reference front() const
		{
			return *cbegin();
		}

		reference back()
		{
			auto tmp = end();
			--tmp;
			return *tmp;
		}

		const_reference back() const
		{
			auto tmp = cend();
			--tmp;
			return *tmp;
		}

		template<class IteratorType, typename std::enable_if<
			std::is_same<IteratorType, typename basic_json_t::iterator>::value or
			std::is_same<IteratorType, typename basic_json_t::const_iterator>::value, int>::type
			= 0>
			IteratorType erase(IteratorType pos)
		{
			IteratorType result = end();

			switch (m_type)
			{
			case value_t::boolean:
			case value_t::number_float:
			case value_t::number_integer:
			case value_t::number_unsigned:
			case value_t::string:
			{
				if (is_string())
				{
					AllocatorType<string_t> alloc;
					std::allocator_traits<decltype(alloc)>::destroy(alloc, m_value.string);
					std::allocator_traits<decltype(alloc)>::deallocate(alloc, m_value.string, 1);
					m_value.string = nullptr;
				}

				m_type = value_t::null;
				break;
			}

			case value_t::object:
			{
				result.m_it.object_iterator = m_value.object->erase(pos.m_it.object_iterator);
				break;
			}

			case value_t::array:
			{
				result.m_it.array_iterator = m_value.array->erase(pos.m_it.array_iterator);
				break;
			}
			}

			return result;
		}

		template<class IteratorType, typename std::enable_if<
			std::is_same<IteratorType, typename basic_json_t::iterator>::value or
			std::is_same<IteratorType, typename basic_json_t::const_iterator>::value, int>::type
			= 0>
			IteratorType erase(IteratorType first, IteratorType last)
		{
			IteratorType result = end();

			switch (m_type)
			{
			case value_t::boolean:
			case value_t::number_float:
			case value_t::number_integer:
			case value_t::number_unsigned:
			case value_t::string:
			{
				if (is_string())
				{
					AllocatorType<string_t> alloc;
					std::allocator_traits<decltype(alloc)>::destroy(alloc, m_value.string);
					std::allocator_traits<decltype(alloc)>::deallocate(alloc, m_value.string, 1);
					m_value.string = nullptr;
				}

				m_type = value_t::null;
				break;
			}

			case value_t::object:
			{
				result.m_it.object_iterator = m_value.object->erase(first.m_it.object_iterator,
					last.m_it.object_iterator);
				break;
			}

			case value_t::array:
			{
				result.m_it.array_iterator = m_value.array->erase(first.m_it.array_iterator,
					last.m_it.array_iterator);
				break;
			}
			}

			return result;
		}

		size_type erase(const typename object_t::key_type& key)
		{
			if (JSON_LIKELY(is_object()))
			{
				return m_value.object->erase(key);
			}
		}

		void erase(const size_type idx)
		{
			if (JSON_LIKELY(is_array()))
			{
				m_value.array->erase(m_value.array->begin() + static_cast<difference_type>(idx));
			}
		}

		template<typename KeyT>
		iterator find(KeyT&& key)
		{
			auto result = end();

			if (is_object())
			{
				result.m_it.object_iterator = m_value.object->find(std::forward<KeyT>(key));
			}

			return result;
		}

		template<typename KeyT>
		const_iterator find(KeyT&& key) const
		{
			auto result = cend();

			if (is_object())
			{
				result.m_it.object_iterator = m_value.object->find(std::forward<KeyT>(key));
			}

			return result;
		}

		template<typename KeyT>
		size_type count(KeyT&& key) const
		{
			return is_object() ? m_value.object->count(std::forward<KeyT>(key)) : 0;
		}

		iterator begin() noexcept
		{
			iterator result(this);
			result.set_begin();
			return result;
		}

		const_iterator begin() const noexcept
		{
			return cbegin();
		}

		const_iterator cbegin() const noexcept
		{
			const_iterator result(this);
			result.set_begin();
			return result;
		}

		iterator end() noexcept
		{
			iterator result(this);
			result.set_end();
			return result;
		}

		const_iterator end() const noexcept
		{
			return cend();
		}

		const_iterator cend() const noexcept
		{
			const_iterator result(this);
			result.set_end();
			return result;
		}

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const noexcept
		{
			return crbegin();
		}

		reverse_iterator rend() noexcept
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const noexcept
		{
			return crend();
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}

		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(cbegin());
		}

	public:
		static iteration_proxy<iterator> iterator_wrapper(reference ref)
		{
			return iteration_proxy<iterator>(ref);
		}

		static iteration_proxy<const_iterator> iterator_wrapper(const_reference ref)
		{
			return iteration_proxy<const_iterator>(ref);
		}

		iteration_proxy<iterator> items()
		{
			return iteration_proxy<iterator>(*this);
		}

		iteration_proxy<const_iterator> items() const
		{
			return iteration_proxy<const_iterator>(*this);
		}

		bool empty() const noexcept
		{
			switch (m_type)
			{
			case value_t::null:
			{
				return true;
			}

			case value_t::array:
			{
				return m_value.array->empty();
			}

			case value_t::object:
			{
				return m_value.object->empty();
			}

			default:
			{
				return false;
			}
			}
		}

		size_type size() const noexcept
		{
			switch (m_type)
			{
			case value_t::null:
			{
				return 0;
			}

			case value_t::array:
			{
				return m_value.array->size();
			}

			case value_t::object:
			{
				return m_value.object->size();
			}

			default:
			{
				return 1;
			}
			}
		}

		size_type max_size() const noexcept
		{
			switch (m_type)
			{
			case value_t::array:
			{
				return m_value.array->max_size();
			}

			case value_t::object:
			{
				return m_value.object->max_size();
			}

			default:
			{
				return size();
			}
			}
		}

		void clear() noexcept
		{
			switch (m_type)
			{
			case value_t::number_integer:
			{
				m_value.number_integer = 0;
				break;
			}

			case value_t::number_unsigned:
			{
				m_value.number_unsigned = 0;
				break;
			}

			case value_t::number_float:
			{
				m_value.number_float = 0.0;
				break;
			}

			case value_t::boolean:
			{
				m_value.boolean = false;
				break;
			}

			case value_t::string:
			{
				m_value.string->clear();
				break;
			}

			case value_t::array:
			{
				m_value.array->clear();
				break;
			}

			case value_t::object:
			{
				m_value.object->clear();
				break;
			}
			}
		}

		void push_back(basic_json&& val)
		{
			if (is_null())
			{
				m_type = value_t::array;
				m_value = value_t::array;
			}

			m_value.array->push_back(std::move(val));
			val.m_type = value_t::null;
		}

		reference operator+=(basic_json&& val)
		{
			push_back(std::move(val));
			return *this;
		}

		void push_back(const basic_json& val)
		{
			if (is_null())
			{
				m_type = value_t::array;
				m_value = value_t::array;
			}

			m_value.array->push_back(val);
		}

		reference operator+=(const basic_json& val)
		{
			push_back(val);
			return *this;
		}

		void push_back(const typename object_t::value_type& val)
		{
			if (is_null())
			{
				m_type = value_t::object;
				m_value = value_t::object;
			}

			m_value.object->insert(val);
		}

		reference operator+=(const typename object_t::value_type& val)
		{
			push_back(val);
			return *this;
		}

		void push_back(initializer_list_t init)
		{
			if (is_object() and init.size() == 2 and (*init.begin())->is_string())
			{
				basic_json&& key = init.begin()->moved_or_copied();
				push_back(typename object_t::value_type(
					std::move(key.get_ref<string_t&>()), (init.begin() + 1)->moved_or_copied()));
			}
			else
			{
				push_back(basic_json(init));
			}
		}

		reference operator+=(initializer_list_t init)
		{
			push_back(init);
			return *this;
		}

		template<class... Args>
		void emplace_back(Args&& ... args)
		{
			if (is_null())
			{
				m_type = value_t::array;
				m_value = value_t::array;
			}

			m_value.array->emplace_back(std::forward<Args>(args)...);
		}

		template<class... Args>
		std::pair<iterator, bool> emplace(Args&& ... args)
		{
			if (is_null())
			{
				m_type = value_t::object;
				m_value = value_t::object;
			}

			auto res = m_value.object->emplace(std::forward<Args>(args)...);
			auto it = begin();
			it.m_it.object_iterator = res.first;

			return { it, res.second };
		}

		iterator insert(const_iterator pos, const basic_json& val)
		{
			if (JSON_LIKELY(is_array()))
			{
				iterator result(this);
				result.m_it.array_iterator = m_value.array->insert(pos.m_it.array_iterator, val);
				return result;
			}
		}

		iterator insert(const_iterator pos, basic_json&& val)
		{
			return insert(pos, val);
		}

		iterator insert(const_iterator pos, size_type cnt, const basic_json& val)
		{
			if (JSON_LIKELY(is_array()))
			{
				iterator result(this);
				result.m_it.array_iterator = m_value.array->insert(pos.m_it.array_iterator, cnt, val);
				return result;
			}
		}

		iterator insert(const_iterator pos, const_iterator first, const_iterator last)
		{
			iterator result(this);
			result.m_it.array_iterator = m_value.array->insert(
				pos.m_it.array_iterator,
				first.m_it.array_iterator,
				last.m_it.array_iterator);
			return result;
		}

		iterator insert(const_iterator pos, initializer_list_t ilist)
		{
			iterator result(this);
			result.m_it.array_iterator = m_value.array->insert(pos.m_it.array_iterator, ilist.begin(), ilist.end());
			return result;
		}

		void insert(const_iterator first, const_iterator last)
		{
			m_value.object->insert(first.m_it.object_iterator, last.m_it.object_iterator);
		}

		void update(const_reference j)
		{
			if (is_null())
			{
				m_type = value_t::object;
				m_value.object = create<object_t>();
			}

			for (auto it = j.cbegin(); it != j.cend(); ++it)
			{
				m_value.object->operator[](it.key()) = it.value();
			}
		}

		void update(const_iterator first, const_iterator last)
		{
			if (is_null())
			{
				m_type = value_t::object;
				m_value.object = create<object_t>();
			}

			for (auto it = first; it != last; ++it)
			{
				m_value.object->operator[](it.key()) = it.value();
			}
		}

		void swap(reference other) noexcept (
			std::is_nothrow_move_constructible<value_t>::value and
			std::is_nothrow_move_assignable<value_t>::value and
			std::is_nothrow_move_constructible<json_value>::value and
			std::is_nothrow_move_assignable<json_value>::value
			)
		{
			std::swap(m_type, other.m_type);
			std::swap(m_value, other.m_value);
		}

		void swap(array_t& other)
		{
			if (JSON_LIKELY(is_array()))
			{
				std::swap(*(m_value.array), other);
			}
		}

		void swap(object_t& other)
		{
			if (JSON_LIKELY(is_object()))
			{
				std::swap(*(m_value.object), other);
			}
		}

		void swap(string_t& other)
		{
			if (JSON_LIKELY(is_string()))
			{
				std::swap(*(m_value.string), other);
			}
		}

	public:

		friend bool operator==(const_reference lhs, const_reference rhs) noexcept
		{
			const auto lhs_type = lhs.type();
			const auto rhs_type = rhs.type();

			if (lhs_type == rhs_type)
			{
				switch (lhs_type)
				{
				case value_t::array:
					return (*lhs.m_value.array == *rhs.m_value.array);

				case value_t::object:
					return (*lhs.m_value.object == *rhs.m_value.object);

				case value_t::null:
					return true;

				case value_t::string:
					return (*lhs.m_value.string == *rhs.m_value.string);

				case value_t::boolean:
					return (lhs.m_value.boolean == rhs.m_value.boolean);

				case value_t::number_integer:
					return (lhs.m_value.number_integer == rhs.m_value.number_integer);

				case value_t::number_unsigned:
					return (lhs.m_value.number_unsigned == rhs.m_value.number_unsigned);

				case value_t::number_float:
					return (lhs.m_value.number_float == rhs.m_value.number_float);

				default:
					return false;
				}
			}
			else if (lhs_type == value_t::number_integer and rhs_type == value_t::number_float)
			{
				return (static_cast<number_float_t>(lhs.m_value.number_integer) == rhs.m_value.number_float);
			}
			else if (lhs_type == value_t::number_float and rhs_type == value_t::number_integer)
			{
				return (lhs.m_value.number_float == static_cast<number_float_t>(rhs.m_value.number_integer));
			}
			else if (lhs_type == value_t::number_unsigned and rhs_type == value_t::number_float)
			{
				return (static_cast<number_float_t>(lhs.m_value.number_unsigned) == rhs.m_value.number_float);
			}
			else if (lhs_type == value_t::number_float and rhs_type == value_t::number_unsigned)
			{
				return (lhs.m_value.number_float == static_cast<number_float_t>(rhs.m_value.number_unsigned));
			}
			else if (lhs_type == value_t::number_unsigned and rhs_type == value_t::number_integer)
			{
				return (static_cast<number_integer_t>(lhs.m_value.number_unsigned) == rhs.m_value.number_integer);
			}
			else if (lhs_type == value_t::number_integer and rhs_type == value_t::number_unsigned)
			{
				return (lhs.m_value.number_integer == static_cast<number_integer_t>(rhs.m_value.number_unsigned));
			}

			return false;
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator==(const_reference lhs, const ScalarType rhs) noexcept
		{
			return (lhs == basic_json(rhs));
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator==(const ScalarType lhs, const_reference rhs) noexcept
		{
			return (basic_json(lhs) == rhs);
		}

		friend bool operator!=(const_reference lhs, const_reference rhs) noexcept
		{
			return not (lhs == rhs);
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator!=(const_reference lhs, const ScalarType rhs) noexcept
		{
			return (lhs != basic_json(rhs));
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator!=(const ScalarType lhs, const_reference rhs) noexcept
		{
			return (basic_json(lhs) != rhs);
		}

		friend bool operator<(const_reference lhs, const_reference rhs) noexcept
		{
			const auto lhs_type = lhs.type();
			const auto rhs_type = rhs.type();

			if (lhs_type == rhs_type)
			{
				switch (lhs_type)
				{
				case value_t::array:
					return (*lhs.m_value.array) < (*rhs.m_value.array);

				case value_t::object:
					return *lhs.m_value.object < *rhs.m_value.object;

				case value_t::null:
					return false;

				case value_t::string:
					return *lhs.m_value.string < *rhs.m_value.string;

				case value_t::boolean:
					return lhs.m_value.boolean < rhs.m_value.boolean;

				case value_t::number_integer:
					return lhs.m_value.number_integer < rhs.m_value.number_integer;

				case value_t::number_unsigned:
					return lhs.m_value.number_unsigned < rhs.m_value.number_unsigned;

				case value_t::number_float:
					return lhs.m_value.number_float < rhs.m_value.number_float;

				default:
					return false;
				}
			}
			else if (lhs_type == value_t::number_integer and rhs_type == value_t::number_float)
			{
				return static_cast<number_float_t>(lhs.m_value.number_integer) < rhs.m_value.number_float;
			}
			else if (lhs_type == value_t::number_float and rhs_type == value_t::number_integer)
			{
				return lhs.m_value.number_float < static_cast<number_float_t>(rhs.m_value.number_integer);
			}
			else if (lhs_type == value_t::number_unsigned and rhs_type == value_t::number_float)
			{
				return static_cast<number_float_t>(lhs.m_value.number_unsigned) < rhs.m_value.number_float;
			}
			else if (lhs_type == value_t::number_float and rhs_type == value_t::number_unsigned)
			{
				return lhs.m_value.number_float < static_cast<number_float_t>(rhs.m_value.number_unsigned);
			}
			else if (lhs_type == value_t::number_integer and rhs_type == value_t::number_unsigned)
			{
				return lhs.m_value.number_integer < static_cast<number_integer_t>(rhs.m_value.number_unsigned);
			}
			else if (lhs_type == value_t::number_unsigned and rhs_type == value_t::number_integer)
			{
				return static_cast<number_integer_t>(lhs.m_value.number_unsigned) < rhs.m_value.number_integer;
			}

			return operator<(lhs_type, rhs_type);
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator<(const_reference lhs, const ScalarType rhs) noexcept
		{
			return (lhs < basic_json(rhs));
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator<(const ScalarType lhs, const_reference rhs) noexcept
		{
			return (basic_json(lhs) < rhs);
		}

		friend bool operator<=(const_reference lhs, const_reference rhs) noexcept
		{
			return not (rhs < lhs);
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator<=(const_reference lhs, const ScalarType rhs) noexcept
		{
			return (lhs <= basic_json(rhs));
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator<=(const ScalarType lhs, const_reference rhs) noexcept
		{
			return (basic_json(lhs) <= rhs);
		}

		friend bool operator>(const_reference lhs, const_reference rhs) noexcept
		{
			return not (lhs <= rhs);
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator>(const_reference lhs, const ScalarType rhs) noexcept
		{
			return (lhs > basic_json(rhs));
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator>(const ScalarType lhs, const_reference rhs) noexcept
		{
			return (basic_json(lhs) > rhs);
		}

		friend bool operator>=(const_reference lhs, const_reference rhs) noexcept
		{
			return not (lhs < rhs);
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator>=(const_reference lhs, const ScalarType rhs) noexcept
		{
			return (lhs >= basic_json(rhs));
		}

		template<typename ScalarType, typename std::enable_if<
			std::is_scalar<ScalarType>::value, int>::type = 0>
			friend bool operator>=(const ScalarType lhs, const_reference rhs) noexcept
		{
			return (basic_json(lhs) >= rhs);
		}

		friend std::ostream& operator<<(std::ostream& o, const basic_json& j)
		{
			serializer s(detail::output_adapter<char>(o), o.fill());
			s.dump(j, false, false, 0);
			return o;
		}

		friend std::ostream& operator>>(const basic_json& j, std::ostream& o)
		{
			return o << j;
		}

		static basic_json parse(detail::input_adapter i,
			const parser_callback_t cb = nullptr)
		{
			basic_json result;
			parser(i, cb).parse(true, result);
			return result;
		}

		static basic_json parse(detail::input_adapter& i,
			const parser_callback_t cb = nullptr)
		{
			basic_json result;
			parser(i, cb).parse(true, result);
			return result;
		}

		static bool accept(detail::input_adapter i)
		{
			return parser(i).accept(true);
		}

		static bool accept(detail::input_adapter& i)
		{
			return parser(i).accept(true);
		}

		template<class IteratorType, typename std::enable_if<
			std::is_base_of<
			std::random_access_iterator_tag,
			typename std::iterator_traits<IteratorType>::iterator_category>::value, int>::type = 0>
			static basic_json parse(IteratorType first, IteratorType last,
				const parser_callback_t cb = nullptr)
		{
			basic_json result;
			parser(detail::input_adapter(first, last), cb).parse(true, result);
			return result;
		}

		template<class IteratorType, typename std::enable_if<
			std::is_base_of<
			std::random_access_iterator_tag,
			typename std::iterator_traits<IteratorType>::iterator_category>::value, int>::type = 0>
			static bool accept(IteratorType first, IteratorType last)
		{
			return parser(detail::input_adapter(first, last)).accept(true);
		}

		friend std::istream& operator<<(basic_json& j, std::istream& i)
		{
			return operator>>(i, j);
		}

		friend std::istream& operator>>(std::istream& i, basic_json& j)
		{
			parser(detail::input_adapter(i)).parse(false, j);
			return i;
		}

	private:
		value_t m_type = value_t::null;
		json_value m_value = {};
	};
}

namespace std
{
	template<>
	inline void swap(nlohmann::json& j1, nlohmann::json& j2) noexcept(
		is_nothrow_move_constructible<nlohmann::json>::value and
		is_nothrow_move_assignable<nlohmann::json>::value
		)
	{
		j1.swap(j2);
	}
}

#include "detail/macro_unscope.hpp"

#endif

#ifndef NLOHMANN_JSON_DETAIL_ITERATORS_ITER_IMPL_HPP
#define NLOHMANN_JSON_DETAIL_ITERATORS_ITER_IMPL_HPP

#include <ciso646>

#include "detail/iterators/internal_iterator.hpp"
#include "detail/iterators/primitive_iterator.hpp"
#include "detail/macro_scope.hpp"
#include "detail/meta.hpp"
#include "detail/value_t.hpp"

namespace nlohmann
{
	namespace detail
	{
		template<typename IteratorType> class iteration_proxy;
		template<typename BasicJsonType>
		class iter_impl
		{
			friend iter_impl<typename std::conditional<std::is_const<BasicJsonType>::value, typename std::remove_const<BasicJsonType>::type, const BasicJsonType>::type>;
			friend BasicJsonType;
			friend iteration_proxy<iter_impl>;

			using object_t = typename BasicJsonType::object_t;
			using array_t = typename BasicJsonType::array_t;

		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = typename BasicJsonType::value_type;
			using difference_type = typename BasicJsonType::difference_type;
			using pointer = typename std::conditional<std::is_const<BasicJsonType>::value,
				typename BasicJsonType::const_pointer,
				typename BasicJsonType::pointer>::type;
			using reference =
				typename std::conditional<std::is_const<BasicJsonType>::value,
				typename BasicJsonType::const_reference,
				typename BasicJsonType::reference>::type;

			iter_impl() = default;

			explicit iter_impl(pointer object) noexcept : m_object(object)
			{
				switch (m_object->m_type)
				{
				case value_t::object:
				{
					m_it.object_iterator = typename object_t::iterator();
					break;
				}

				case value_t::array:
				{
					m_it.array_iterator = typename array_t::iterator();
					break;
				}

				default:
				{
					break;
				}
				}
			}

			iter_impl(const iter_impl<typename std::remove_const<BasicJsonType>::type>& other) noexcept
				: m_object(other.m_object), m_it(other.m_it) {}

			iter_impl& operator=(const iter_impl<typename std::remove_const<BasicJsonType>::type>& other) noexcept
			{
				m_object = other.m_object;
				m_it = other.m_it;
				return *this;
			}

		private:
			void set_begin() noexcept
			{
				switch (m_object->m_type)
				{
				case value_t::object:
				{
					m_it.object_iterator = m_object->m_value.object->begin();
					break;
				}

				case value_t::array:
				{
					m_it.array_iterator = m_object->m_value.array->begin();
					break;
				}
				
				default:
				{
					break;
				}
				}
			}

			void set_end() noexcept
			{
				switch (m_object->m_type)
				{
				case value_t::object:
				{
					m_it.object_iterator = m_object->m_value.object->end();
					break;
				}

				case value_t::array:
				{
					m_it.array_iterator = m_object->m_value.array->end();
					break;
				}
				}
			}

		public:
			reference operator*() const
			{
				switch (m_object->m_type)
				{
				case value_t::object:
				{
					return m_it.object_iterator->second;
				}

				case value_t::array:
				{
					return *m_it.array_iterator;
				}
				}
			}

			pointer operator->() const
			{
				switch (m_object->m_type)
				{
				case value_t::object:
				{
					return &(m_it.object_iterator->second);
				}

				case value_t::array:
				{
					return &*m_it.array_iterator;
				}
				}
			}

			iter_impl const operator++(int)
			{
				auto result = *this;
				++(*this);
				return result;
			}

			iter_impl& operator++()
			{
				switch (m_object->m_type)
				{
				case value_t::object:
				{
					std::advance(m_it.object_iterator, 1);
					break;
				}

				case value_t::array:
				{
					std::advance(m_it.array_iterator, 1);
					break;
				}
				}

				return *this;
			}

			iter_impl const operator--(int)
			{
				auto result = *this;
				--(*this);
				return result;
			}

			iter_impl& operator--()
			{
				switch (m_object->m_type)
				{
				case value_t::object:
				{
					std::advance(m_it.object_iterator, -1);
					break;
				}

				case value_t::array:
				{
					std::advance(m_it.array_iterator, -1);
					break;
				}
				}

				return *this;
			}

			bool operator==(const iter_impl& other) const
			{
				switch (m_object->m_type)
				{
				case value_t::object:
					return (m_it.object_iterator == other.m_it.object_iterator);

				case value_t::array:
					return (m_it.array_iterator == other.m_it.array_iterator);
				}
			}

			bool operator!=(const iter_impl& other) const
			{
				return not operator==(other);
			}

			bool operator<(const iter_impl& other) const
			{
				switch (m_object->m_type)
				{
				case value_t::array:
					return (m_it.array_iterator < other.m_it.array_iterator);
				}
			}

			bool operator<=(const iter_impl& other) const
			{
				return not other.operator < (*this);
			}

			bool operator>(const iter_impl& other) const
			{
				return not operator<=(other);
			}

			bool operator>=(const iter_impl& other) const
			{
				return not operator<(other);
			}

			iter_impl& operator+=(difference_type i)
			{
				switch (m_object->m_type)
				{
				case value_t::array:
				{
					std::advance(m_it.array_iterator, i);
					break;
				}
				}

				return *this;
			}

			iter_impl& operator-=(difference_type i)
			{
				return operator+=(-i);
			}

			iter_impl operator+(difference_type i) const
			{
				auto result = *this;
				result += i;
				return result;
			}

			friend iter_impl operator+(difference_type i, const iter_impl& it)
			{
				auto result = it;
				result += i;
				return result;
			}

			iter_impl operator-(difference_type i) const
			{
				auto result = *this;
				result -= i;
				return result;
			}

			difference_type operator-(const iter_impl& other) const
			{
				switch (m_object->m_type)
				{
				case value_t::array:
					return m_it.array_iterator - other.m_it.array_iterator;
				}
			}

			reference operator[](difference_type n) const
			{
				switch (m_object->m_type)
				{
				case value_t::array:
					return *std::next(m_it.array_iterator, n);
				}
			}

			typename object_t::key_type key() const
			{
				if (JSON_LIKELY(m_object->is_object()))
					return m_it.object_iterator->first;
			}

			reference value() const
			{
				return operator*();
			}

		private:
			pointer m_object = nullptr;
			internal_iterator<typename std::remove_const<BasicJsonType>::type> m_it = {};
		};
	}
}

#endif

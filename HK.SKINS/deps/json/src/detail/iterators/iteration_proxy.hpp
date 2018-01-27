#ifndef NLOHMANN_JSON_DETAIL_ITERATORS_ITERATION_PROXY_HPP
#define NLOHMANN_JSON_DETAIL_ITERATORS_ITERATION_PROXY_HPP

#include <string>

#include "detail/value_t.hpp"

namespace nlohmann
{
	namespace detail
	{
		template<typename IteratorType> class iteration_proxy
		{
		private:
			class iteration_proxy_internal
			{
			private:
				IteratorType anchor;
				std::size_t array_index = 0;

			public:
				explicit iteration_proxy_internal(IteratorType it) noexcept : anchor(it) {}

				iteration_proxy_internal& operator*()
				{
					return *this;
				}

				iteration_proxy_internal& operator++()
				{
					++anchor;
					++array_index;

					return *this;
				}

				bool operator!=(const iteration_proxy_internal& o) const noexcept
				{
					return anchor != o.anchor;
				}

				std::string key() const
				{
					switch (anchor.m_object->type())
					{
					case value_t::array:
						return std::to_string(array_index);

					case value_t::object:
						return anchor.key();
					}

					return {};
				}

				typename IteratorType::reference value() const
				{
					return anchor.value();
				}
			};

			typename IteratorType::reference container;

		public:
			explicit iteration_proxy(typename IteratorType::reference cont)
				: container(cont) {}

			iteration_proxy_internal begin() noexcept
			{
				return iteration_proxy_internal(container.begin());
			}

			iteration_proxy_internal end() noexcept
			{
				return iteration_proxy_internal(container.end());
			}
		};
	}
}

#endif

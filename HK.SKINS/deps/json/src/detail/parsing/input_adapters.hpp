#ifndef NLOHMANN_JSON_DETAIL_PARSING_INPUT_ADAPTERS_HPP
#define NLOHMANN_JSON_DETAIL_PARSING_INPUT_ADAPTERS_HPP

#include <string>

#include "detail/macro_scope.hpp"

namespace nlohmann
{
	namespace detail
	{
		struct input_adapter_protocol
		{
			virtual std::char_traits<char>::int_type get_character() = 0;
			virtual void unget_character() = 0;
			virtual ~input_adapter_protocol() = default;
		};

		using input_adapter_t = std::shared_ptr<input_adapter_protocol>;

		class input_stream_adapter : public input_adapter_protocol
		{
		public:
			~input_stream_adapter() override
			{
				is.clear();
			}

			explicit input_stream_adapter(std::istream& i)
				: is(i), sb(*i.rdbuf())
			{
				std::char_traits<char>::int_type c;
				if ((c = get_character()) == 0xEF)
				{
					if ((c = get_character()) == 0xBB)
					{
						if ((c = get_character()) == 0xBF)
						{
							return;
						}
						else if (c != std::char_traits<char>::eof())
						{
							is.unget();
						}
						is.putback('\xBB');
					}
					else if (c != std::char_traits<char>::eof())
					{
						is.unget();
					}
					is.putback('\xEF');
				}
				else if (c != std::char_traits<char>::eof())
				{
					is.unget();
				}
			}

			input_stream_adapter(const input_stream_adapter&) = delete;
			input_stream_adapter& operator=(input_stream_adapter&) = delete;

			std::char_traits<char>::int_type get_character() override
			{
				return sb.sbumpc();
			}

			void unget_character() override
			{
				sb.sungetc();
			}

		private:
			std::istream& is;
			std::streambuf& sb;
		};

		class input_buffer_adapter : public input_adapter_protocol
		{
		public:
			input_buffer_adapter(const char* b, const std::size_t l)
				: cursor(b), limit(b + l), start(b)
			{
				if (l >= 3 and b[0] == '\xEF' and b[1] == '\xBB' and b[2] == '\xBF')
				{
					cursor += 3;
				}
			}

			input_buffer_adapter(const input_buffer_adapter&) = delete;
			input_buffer_adapter& operator=(input_buffer_adapter&) = delete;

			std::char_traits<char>::int_type get_character() noexcept override
			{
				if (JSON_LIKELY(cursor < limit))
				{
					return std::char_traits<char>::to_int_type(*(cursor++));
				}

				return std::char_traits<char>::eof();
			}

			void unget_character() noexcept override
			{
				if (JSON_LIKELY(cursor > start))
				{
					--cursor;
				}
			}

		private:
			const char* cursor;
			const char* limit;
			const char* start;
		};

		class input_adapter
		{
		public:
			input_adapter(std::istream& i)
				: ia(std::make_shared<input_stream_adapter>(i)) {}

			input_adapter(std::istream&& i)
				: ia(std::make_shared<input_stream_adapter>(i)) {}

			template<typename CharT,
				typename std::enable_if<
				std::is_pointer<CharT>::value and
				std::is_integral<typename std::remove_pointer<CharT>::type>::value and
				sizeof(typename std::remove_pointer<CharT>::type) == 1,
				int>::type = 0>
				input_adapter(CharT b, std::size_t l)
				: ia(std::make_shared<input_buffer_adapter>(reinterpret_cast<const char*>(b), l)) {}

			template<typename CharT,
				typename std::enable_if<
				std::is_pointer<CharT>::value and
				std::is_integral<typename std::remove_pointer<CharT>::type>::value and
				sizeof(typename std::remove_pointer<CharT>::type) == 1,
				int>::type = 0>
				input_adapter(CharT b)
				: input_adapter(reinterpret_cast<const char*>(b),
					std::strlen(reinterpret_cast<const char*>(b))) {}

			template<class IteratorType,
				typename std::enable_if<
				std::is_same<typename std::iterator_traits<IteratorType>::iterator_category, std::random_access_iterator_tag>::value,
				int>::type = 0>
				input_adapter(IteratorType first, IteratorType last)
			{
				const auto len = static_cast<size_t>(std::distance(first, last));
				if (JSON_LIKELY(len > 0))
				{
					ia = std::make_shared<input_buffer_adapter>(reinterpret_cast<const char*>(&(*first)), len);
				}
				else
				{
					ia = std::make_shared<input_buffer_adapter>(nullptr, len);
				}
			}

			template<class T, std::size_t N>
			input_adapter(T(&array)[N])
				: input_adapter(std::begin(array), std::end(array)) {}

			template<class ContiguousContainer, typename
				std::enable_if<not std::is_pointer<ContiguousContainer>::value and
				std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<decltype(std::begin(std::declval<ContiguousContainer const>()))>::iterator_category>::value,
				int>::type = 0>
				input_adapter(const ContiguousContainer& c)
				: input_adapter(std::begin(c), std::end(c)) {}

			operator input_adapter_t()
			{
				return ia;
			}

		private:
			input_adapter_t ia = nullptr;
		};
	}
}

#endif

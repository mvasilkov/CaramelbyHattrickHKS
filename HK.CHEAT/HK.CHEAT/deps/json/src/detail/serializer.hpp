#ifndef NLOHMANN_JSON_DETAIL_SERIALIZER_HPP
#define NLOHMANN_JSON_DETAIL_SERIALIZER_HPP

#include <ciso646>
#include <string>

#include "detail/macro_scope.hpp"
#include "detail/meta.hpp"
#include "detail/parsing/output_adapters.hpp"
#include "detail/value_t.hpp"

namespace nlohmann
{
	namespace detail
	{
		template<typename BasicJsonType>
		class serializer
		{
			using string_t = typename BasicJsonType::string_t;
			using number_float_t = typename BasicJsonType::number_float_t;
			using number_integer_t = typename BasicJsonType::number_integer_t;
			using number_unsigned_t = typename BasicJsonType::number_unsigned_t;
		public:

			serializer(output_adapter_t<char> s, const char ichar)
				: o(std::move(s)), indent_char(ichar), indent_string(512, indent_char) {}
			serializer(const serializer&) = delete;
			serializer& operator=(const serializer&) = delete;

			void dump(const BasicJsonType& val, const bool pretty_print,
				const bool ensure_ascii,
				const unsigned int indent_step,
				const unsigned int current_indent = 0)
			{
				switch (val.m_type)
				{
				case value_t::object:
				{
					if (val.m_value.object->empty())
					{
						o->write_characters(XorString("{}"), 2);
						return;
					}
					o->write_character('{');
					auto i = val.m_value.object->cbegin();
					for (std::size_t cnt = 0; cnt < val.m_value.object->size() - 1; ++cnt, ++i)
					{
						o->write_character('\"');
						dump_escaped(i->first, ensure_ascii);
						o->write_characters(XorString("\":"), 2);
						dump(i->second, false, ensure_ascii, indent_step, current_indent);
						o->write_character(',');
					}
					o->write_character('\"');
					dump_escaped(i->first, ensure_ascii);
					o->write_characters(XorString("\":"), 2);
					dump(i->second, false, ensure_ascii, indent_step, current_indent);
					o->write_character('}');
					return;
				}

				case value_t::array:
				{
					if (val.m_value.array->empty())
					{
						o->write_characters(XorString("[]"), 2);
						return;
					}
					o->write_character('[');
					for (auto i = val.m_value.array->cbegin();
						i != val.m_value.array->cend() - 1; ++i)
					{
						dump(*i, false, ensure_ascii, indent_step, current_indent);
						o->write_character(',');
					}
					dump(val.m_value.array->back(), false, ensure_ascii, indent_step, current_indent);
					o->write_character(']');
					return;
				}
				case value_t::string:
				{
					o->write_character('\"');
					dump_escaped(*val.m_value.string, ensure_ascii);
					o->write_character('\"');
					return;
				}
				case value_t::boolean:
				{
					if (val.m_value.boolean)
					{
						o->write_characters(XorString("true"), 4);
					}
					else
					{
						o->write_characters(XorString("false"), 5);
					}
					return;
				}
				case value_t::number_integer:
				{
					dump_integer(val.m_value.number_integer);
					return;
				}
				case value_t::number_unsigned:
				{
					dump_integer(val.m_value.number_unsigned);
					return;
				}
				case value_t::number_float:
				{
					dump_float(val.m_value.number_float);
					return;
				}
				case value_t::null:
				{
					o->write_characters(XorString("null"), 4);
					return;
				}
				}
			}

		private:
			static std::size_t extra_space(const string_t& s,
				const bool ensure_ascii) noexcept
			{
				std::size_t res = 0;

				for (std::size_t i = 0; i < s.size(); ++i)
				{
					switch (s[i])
					{
					case '"':
					case '\\':
					case '\b':
					case '\f':
					case '\n':
					case '\r':
					case '\t':
					{
						res += 1;
						break;
					}

					case 0x00:
					case 0x01:
					case 0x02:
					case 0x03:
					case 0x04:
					case 0x05:
					case 0x06:
					case 0x07:
					case 0x0B:
					case 0x0E:
					case 0x0F:
					case 0x10:
					case 0x11:
					case 0x12:
					case 0x13:
					case 0x14:
					case 0x15:
					case 0x16:
					case 0x17:
					case 0x18:
					case 0x19:
					case 0x1A:
					case 0x1B:
					case 0x1C:
					case 0x1D:
					case 0x1E:
					case 0x1F:
					{
						res += 5;
						break;
					}
					}
				}

				return res;
			}

			void dump_escaped(const string_t& s, const bool ensure_ascii) const
			{
				const auto space = extra_space(s, ensure_ascii);
				if (space == 0)
				{
					o->write_characters(s.c_str(), s.size());
					return;
				}

				string_t result(s.size() + space, '\\');
				std::size_t pos = 0;

				for (std::size_t i = 0; i < s.size(); ++i)
				{
					switch (s[i])
					{
					case '"':
					{
						result[pos + 1] = '"';
						pos += 2;
						break;
					}

					case '\\':
					{
						pos += 2;
						break;
					}

					case '\b':
					{
						result[pos + 1] = 'b';
						pos += 2;
						break;
					}

					case '\f':
					{
						result[pos + 1] = 'f';
						pos += 2;
						break;
					}

					case '\n':
					{
						result[pos + 1] = 'n';
						pos += 2;
						break;
					}

					case '\r':
					{
						result[pos + 1] = 'r';
						pos += 2;
						break;
					}

					case '\t':
					{
						result[pos + 1] = 't';
						pos += 2;
						break;
					}
					}
				}

				o->write_characters(result.c_str(), result.size());
			}

			template<typename NumberType, detail::enable_if_t<
				std::is_same<NumberType, number_unsigned_t>::value or
				std::is_same<NumberType, number_integer_t>::value,
				int> = 0>
				void dump_integer(NumberType x)
			{
				if (x == 0)
				{
					o->write_character('0');
					return;
				}

				const bool is_negative = (x <= 0) and (x != 0);
				std::size_t i = 0;

				while (x != 0)
				{
					const auto digit = std::labs(static_cast<long>(x % 10));
					number_buffer[i++] = static_cast<char>('0' + digit);
					x /= 10;
				}

				if (is_negative) number_buffer[i++] = '-';
				std::reverse(number_buffer.begin(), number_buffer.begin() + i);
				o->write_characters(number_buffer.data(), i);
			}

			void dump_float(number_float_t x)
			{
				xprintf(number_buffer.data(), number_buffer.size(), XorString("%f"), x);
				o->write_characters(number_buffer.data(), static_cast<std::size_t>(strlen(number_buffer.data())));
			}

		private:
			output_adapter_t<char> o = nullptr;
			std::array<char, (XS_RANDOM_SIZE / 4)> number_buffer{ {} };
			const char indent_char;
			string_t indent_string;
		};
	}
}

#endif

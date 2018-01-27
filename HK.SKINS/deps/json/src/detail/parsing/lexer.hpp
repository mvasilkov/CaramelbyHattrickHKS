#ifndef NLOHMANN_JSON_DETAIL_PARSING_LEXER_HPP
#define NLOHMANN_JSON_DETAIL_PARSING_LEXER_HPP

#include <string>
#include <vector>

#include "detail/macro_scope.hpp"
#include "detail/parsing/input_adapters.hpp"

namespace nlohmann
{
	namespace detail
	{
		template<typename BasicJsonType>
		class lexer
		{
			using number_integer_t = typename BasicJsonType::number_integer_t;
			using number_unsigned_t = typename BasicJsonType::number_unsigned_t;
			using number_float_t = typename BasicJsonType::number_float_t;

		public:
			enum class token_type
			{
				uninitialized,
				literal_true,
				literal_false,
				literal_null,
				value_string,
				value_unsigned,
				value_integer,
				value_float,
				begin_array,
				begin_object,
				end_array,
				end_object,
				name_separator,
				value_separator,
				parse_error,
				end_of_input,
				literal_or_value,
			};

			explicit lexer(detail::input_adapter_t adapter)
				: ia(std::move(adapter)) {}

			lexer(const lexer&) = delete;
			lexer& operator=(lexer&) = delete;

		private:
			token_type scan_string()
			{
				reset();
				while (true)
				{
					switch (get())
					{
					case '\"':
					{
						return token_type::value_string;
					}
					case '\\':
					{
						switch (get())
						{
						case '\"':
							add('\"');
							break;
						case '\\':
							add('\\');
							break;
						case '/':
							add('/');
							break;
						case 'b':
							add('\b');
							break;
						case 'f':
							add('\f');
							break;
						case 'n':
							add('\n');
							break;
						case 'r':
							add('\r');
							break;
						case 't':
							add('\t');
							break;
						default:
							return token_type::parse_error;
						}
						break;
					}
					case 0x20:
					case 0x21:
					case 0x23:
					case 0x24:
					case 0x25:
					case 0x26:
					case 0x27:
					case 0x28:
					case 0x29:
					case 0x2A:
					case 0x2B:
					case 0x2C:
					case 0x2D:
					case 0x2E:
					case 0x2F:
					case 0x30:
					case 0x31:
					case 0x32:
					case 0x33:
					case 0x34:
					case 0x35:
					case 0x36:
					case 0x37:
					case 0x38:
					case 0x39:
					case 0x3A:
					case 0x3B:
					case 0x3C:
					case 0x3D:
					case 0x3E:
					case 0x3F:
					case 0x40:
					case 0x41:
					case 0x42:
					case 0x43:
					case 0x44:
					case 0x45:
					case 0x46:
					case 0x47:
					case 0x48:
					case 0x49:
					case 0x4A:
					case 0x4B:
					case 0x4C:
					case 0x4D:
					case 0x4E:
					case 0x4F:
					case 0x50:
					case 0x51:
					case 0x52:
					case 0x53:
					case 0x54:
					case 0x55:
					case 0x56:
					case 0x57:
					case 0x58:
					case 0x59:
					case 0x5A:
					case 0x5B:
					case 0x5D:
					case 0x5E:
					case 0x5F:
					case 0x60:
					case 0x61:
					case 0x62:
					case 0x63:
					case 0x64:
					case 0x65:
					case 0x66:
					case 0x67:
					case 0x68:
					case 0x69:
					case 0x6A:
					case 0x6B:
					case 0x6C:
					case 0x6D:
					case 0x6E:
					case 0x6F:
					case 0x70:
					case 0x71:
					case 0x72:
					case 0x73:
					case 0x74:
					case 0x75:
					case 0x76:
					case 0x77:
					case 0x78:
					case 0x79:
					case 0x7A:
					case 0x7B:
					case 0x7C:
					case 0x7D:
					case 0x7E:
					case 0x7F:
					{
						add(current);
						break;
					}
					}
				}
			}

			static void strtof(float& f, const char* str, char** endptr) noexcept
			{
				f = std::strtof(str, endptr);
			}

			static void strtof(double& f, const char* str, char** endptr) noexcept
			{
				f = std::strtod(str, endptr);
			}

			token_type scan_number()
			{
				reset();
				token_type number_type = token_type::value_unsigned;

				switch (current)
				{
				case '-':
				{
					add(current);
					goto scan_number_minus;
				}

				case '0':
				{
					add(current);
					goto scan_number_zero;
				}

				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					add(current);
					goto scan_number_any1;
				}
				}

			scan_number_minus:
				number_type = token_type::value_integer;
				switch (get())
				{
				case '0':
				{
					add(current);
					goto scan_number_zero;
				}

				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					add(current);
					goto scan_number_any1;
				}

				default:
				{
					return token_type::parse_error;
				}
				}

			scan_number_zero:
				switch (get())
				{
				case '.':
				{
					add(decimal_point_char);
					goto scan_number_decimal1;
				}

				default:
					goto scan_number_done;
				}

			scan_number_any1:
				switch (get())
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					add(current);
					goto scan_number_any1;
				}

				case '.':
				{
					add(decimal_point_char);
					goto scan_number_decimal1;
				}

				default:
					goto scan_number_done;
				}

			scan_number_decimal1:
				number_type = token_type::value_float;
				switch (get())
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					add(current);
					goto scan_number_decimal2;
				}

				default:
				{
					return token_type::parse_error;
				}
				}

			scan_number_decimal2:
				switch (get())
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					add(current);
					goto scan_number_decimal2;
				}

				default:
					goto scan_number_done;
				}
			scan_number_sign:
				switch (get())
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					add(current);
					goto scan_number_any2;
				}

				default:
				{
					return token_type::parse_error;
				}
				}

			scan_number_any2:
				switch (get())
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					add(current);
					goto scan_number_any2;
				}

				default:
					goto scan_number_done;
				}

			scan_number_done:
				unget();

				char* endptr = nullptr;
				errno = 0;

				if (number_type == token_type::value_unsigned)
				{
					const auto x = std::strtoull(yytext.data(), &endptr, 10);

					if (errno == 0)
					{
						value_unsigned = static_cast<number_unsigned_t>(x);
						if (value_unsigned == x)
						{
							return token_type::value_unsigned;
						}
					}
				}
				else if (number_type == token_type::value_integer)
				{
					const auto x = std::strtoll(yytext.data(), &endptr, 10);

					if (errno == 0)
					{
						value_integer = static_cast<number_integer_t>(x);
						if (value_integer == x)
						{
							return token_type::value_integer;
						}
					}
				}

				strtof(value_float, yytext.data(), &endptr);
				return token_type::value_float;
			}

			token_type scan_literal(const char* literal_text, const std::size_t length,
				token_type return_type)
			{
				for (std::size_t i = 1; i < length; ++i)
				{
					if (JSON_UNLIKELY(get() != literal_text[i]))
					{
						return token_type::parse_error;
					}
				}
				return return_type;
			}

			void reset() noexcept
			{
				yytext.clear();
				token_string.clear();
				token_string.push_back(std::char_traits<char>::to_char_type(current));
			}

			std::char_traits<char>::int_type get()
			{
				++chars_read;
				current = ia->get_character();
				if (JSON_LIKELY(current != std::char_traits<char>::eof()))
				{
					token_string.push_back(std::char_traits<char>::to_char_type(current));
				}
				return current;
			}

			void unget()
			{
				--chars_read;
				if (JSON_LIKELY(current != std::char_traits<char>::eof()))
				{
					ia->unget_character();
					token_string.pop_back();
				}
			}

			void add(int c)
			{
				yytext.push_back(std::char_traits<char>::to_char_type(c));
			}

		public:
			constexpr number_integer_t get_number_integer() const noexcept
			{
				return value_integer;
			}

			constexpr number_unsigned_t get_number_unsigned() const noexcept
			{
				return value_unsigned;
			}

			constexpr number_float_t get_number_float() const noexcept
			{
				return value_float;
			}

			std::string move_string()
			{
				return std::move(yytext);
			}

			constexpr std::size_t get_position() const noexcept
			{
				return chars_read;
			}

			token_type scan()
			{
				do
				{
					get();
				} while (current == ' ' or current == '\t' or current == '\n' or current == '\r');

				switch (current)
				{
				case '[':
					return token_type::begin_array;
				case ']':
					return token_type::end_array;
				case '{':
					return token_type::begin_object;
				case '}':
					return token_type::end_object;
				case ':':
					return token_type::name_separator;
				case ',':
					return token_type::value_separator;

				case 't':
					return scan_literal(XorString("true"), 4, token_type::literal_true);
				case 'f':
					return scan_literal(XorString("false"), 5, token_type::literal_false);
				case 'n':
					return scan_literal(XorString("null"), 4, token_type::literal_null);

				case '\"':
					return scan_string();

				case '-':
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					return scan_number();

				case '\0':
				case std::char_traits<char>::eof():
					return token_type::end_of_input;

				default:
					return token_type::parse_error;
				}
			}

		private:
			detail::input_adapter_t ia = nullptr;
			std::char_traits<char>::int_type current = std::char_traits<char>::eof();
			std::size_t chars_read = 0;
			std::vector<char> token_string{};
			std::string yytext{};
			number_integer_t value_integer = 0;
			number_unsigned_t value_unsigned = 0;
			number_float_t value_float = 0;
			const char decimal_point_char = '.';
		};
	}
}

#endif

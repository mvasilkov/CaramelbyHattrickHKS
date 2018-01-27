#ifndef NLOHMANN_JSON_DETAIL_PARSING_PARSER_HPP
#define NLOHMANN_JSON_DETAIL_PARSING_PARSER_HPP

#include <string>

#include "detail/macro_scope.hpp"
#include "detail/parsing/input_adapters.hpp"
#include "detail/parsing/lexer.hpp"
#include "detail/value_t.hpp"

namespace nlohmann
{
	namespace detail
	{
		template<typename BasicJsonType>
		class parser
		{
			using number_integer_t = typename BasicJsonType::number_integer_t;
			using number_unsigned_t = typename BasicJsonType::number_unsigned_t;
			using number_float_t = typename BasicJsonType::number_float_t;
			using lexer_t = lexer<BasicJsonType>;
			using token_type = typename lexer_t::token_type;

		public:
			enum class parse_event_t : uint8_t
			{
				object_start,
				object_end,
				array_start,
				array_end,
				key,
				value,
			};

			using parser_callback_t =
				std::function<bool(int depth, parse_event_t event, BasicJsonType& parsed)>;

			explicit parser(detail::input_adapter_t adapter,
				const parser_callback_t cb = nullptr)
				: callback(cb), m_lexer(adapter)
			{}

			void parse(const bool strict, BasicJsonType& result)
			{
				get_token();
				parse_internal(true, result);

				if (strict)
				{
					get_token();
					expect(token_type::end_of_input);
				}
			}

			bool accept(const bool strict = true)
			{
				get_token();

				if (not accept_internal())
					return false;

				return not strict or (get_token() == token_type::end_of_input);
			}

		private:
			void parse_internal(bool keep, BasicJsonType& result)
			{
				result.m_value.destroy(result.m_type);
				result.m_type = value_t::null;

				switch (last_token)
				{
				case token_type::begin_object:
				{
					if (keep)
					{
						if (callback)
						{
							keep = callback(depth++, parse_event_t::object_start, result);
						}

						if (not callback or keep)
						{
							result.m_type = value_t::object;
							result.m_value = value_t::object;
						}
					}

					get_token();

					if (last_token == token_type::end_object)
					{
						if (keep and callback and not callback(--depth, parse_event_t::object_end, result))
						{
							result.m_value.destroy(result.m_type);
						}
						break;
					}

					std::string key;
					BasicJsonType value;
					while (true)
					{
						if (not expect(token_type::value_string))
						{
							return;
						}
						key = m_lexer.move_string();

						bool keep_tag = false;
						if (keep)
						{
							if (callback)
							{
								BasicJsonType k(key);
								keep_tag = callback(depth, parse_event_t::key, k);
							}
							else
							{
								keep_tag = true;
							}
						}

						get_token();
						if (not expect(token_type::name_separator))
						{
							return;
						}

						get_token();
						value.m_value.destroy(value.m_type);
						parse_internal(keep, value);

						if (JSON_UNLIKELY(errored))
						{
							return;
						}

						if (keep and keep_tag)
						{
							result.m_value.object->emplace(std::move(key), std::move(value));
						}

						get_token();
						if (last_token == token_type::value_separator)
						{
							get_token();
							continue;
						}

						if (not expect(token_type::end_object))
						{
							return;
						}
						break;
					}

					if (keep and callback and not callback(--depth, parse_event_t::object_end, result))
					{
						result.m_value.destroy(result.m_type);
					}
					break;
				}

				case token_type::begin_array:
				{
					if (keep)
					{
						if (callback)
						{
							keep = callback(depth++, parse_event_t::array_start, result);
						}

						if (not callback or keep)
						{
							result.m_type = value_t::array;
							result.m_value = value_t::array;
						}
					}

					get_token();

					if (last_token == token_type::end_array)
					{
						if (callback and not callback(--depth, parse_event_t::array_end, result))
						{
							result.m_value.destroy(result.m_type);
						}
						break;
					}

					BasicJsonType value;
					while (true)
					{
						value.m_value.destroy(value.m_type);
						parse_internal(keep, value);

						if (JSON_UNLIKELY(errored))
						{
							return;
						}

						if (keep)
						{
							result.m_value.array->push_back(std::move(value));
						}

						get_token();
						if (last_token == token_type::value_separator)
						{
							get_token();
							continue;
						}

						if (not expect(token_type::end_array))
						{
							return;
						}
						break;
					}

					if (keep and callback and not callback(--depth, parse_event_t::array_end, result))
					{
						result.m_value.destroy(result.m_type);
					}
					break;
				}

				case token_type::literal_null:
				{
					result.m_type = value_t::null;
					break;
				}

				case token_type::value_string:
				{
					result.m_type = value_t::string;
					result.m_value = m_lexer.move_string();
					break;
				}

				case token_type::literal_true:
				{
					result.m_type = value_t::boolean;
					result.m_value = true;
					break;
				}

				case token_type::literal_false:
				{
					result.m_type = value_t::boolean;
					result.m_value = false;
					break;
				}

				case token_type::value_unsigned:
				{
					result.m_type = value_t::number_unsigned;
					result.m_value = m_lexer.get_number_unsigned();
					break;
				}

				case token_type::value_integer:
				{
					result.m_type = value_t::number_integer;
					result.m_value = m_lexer.get_number_integer();
					break;
				}

				case token_type::value_float:
				{
					result.m_type = value_t::number_float;
					result.m_value = m_lexer.get_number_float();

					if (JSON_UNLIKELY(not std::isfinite(result.m_value.number_float)))
					{
						expect(token_type::uninitialized);
					}
					break;
				}

				case token_type::parse_error:
				{
					if (not expect(token_type::uninitialized))
					{
						return;
					}
					break;
				}

				default:
				{
					if (not expect(token_type::literal_or_value))
					{
						return;
					}
					break;
				}
				}
			}

			bool accept_internal()
			{
				switch (last_token)
				{
				case token_type::begin_object:
				{
					get_token();

					if (last_token == token_type::end_object)
					{
						return true;
					}

					while (true)
					{
						if (last_token != token_type::value_string)
						{
							return false;
						}

						get_token();
						if (last_token != token_type::name_separator)
						{
							return false;
						}

						get_token();
						if (not accept_internal())
						{
							return false;
						}

						get_token();
						if (last_token == token_type::value_separator)
						{
							get_token();
							continue;
						}

						return (last_token == token_type::end_object);
					}
				}

				case token_type::begin_array:
				{
					get_token();

					if (last_token == token_type::end_array)
					{
						return true;
					}

					while (true)
					{
						if (not accept_internal())
						{
							return false;
						}

						get_token();
						if (last_token == token_type::value_separator)
						{
							get_token();
							continue;
						}

						return (last_token == token_type::end_array);
					}
				}

				case token_type::value_float:
				{
					return std::isfinite(m_lexer.get_number_float());
				}

				case token_type::literal_false:
				case token_type::literal_null:
				case token_type::literal_true:
				case token_type::value_integer:
				case token_type::value_string:
				case token_type::value_unsigned:
					return true;

				default:
					return false;
				}
			}

			token_type get_token()
			{
				return (last_token = m_lexer.scan());
			}

			bool expect(token_type t)
			{
				if (JSON_UNLIKELY(t != last_token))
				{
					errored = true;
					expected = t;
					return false;
				}

				return true;
			}

		private:
			int depth = 0;
			const parser_callback_t callback = nullptr;
			token_type last_token = token_type::uninitialized;
			lexer_t m_lexer;
			bool errored = false;
			token_type expected = token_type::uninitialized;
		};
	}
}

#endif

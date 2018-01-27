#ifndef NLOHMANN_JSON_DETAIL_PARSING_OUTPUT_ADAPTERS_HPP
#define NLOHMANN_JSON_DETAIL_PARSING_OUTPUT_ADAPTERS_HPP

#include <string>
#include <vector>

namespace nlohmann
{
	namespace detail
	{
		template<typename CharType> struct output_adapter_protocol
		{
			virtual void write_character(CharType c) = 0;
			virtual void write_characters(const CharType* s, std::size_t length) = 0;
			virtual ~output_adapter_protocol() = default;
		};

		template<typename CharType>
		using output_adapter_t = std::shared_ptr<output_adapter_protocol<CharType>>;

		template<typename CharType>
		class output_vector_adapter : public output_adapter_protocol<CharType>
		{
		public:
			explicit output_vector_adapter(std::vector<CharType>& vec) : v(vec) {}

			void write_character(CharType c) override
			{
				v.push_back(c);
			}

			void write_characters(const CharType* s, std::size_t length) override
			{
				std::copy(s, s + length, std::back_inserter(v));
			}

		private:
			std::vector<CharType>& v;
		};

		template<typename CharType>
		class output_stream_adapter : public output_adapter_protocol<CharType>
		{
		public:
			explicit output_stream_adapter(std::basic_ostream<CharType>& s) : stream(s) {}

			void write_character(CharType c) override
			{
				stream.put(c);
			}

			void write_characters(const CharType* s, std::size_t length) override
			{
				stream.write(s, static_cast<std::streamsize>(length));
			}

		private:
			std::basic_ostream<CharType>& stream;
		};

		template<typename CharType>
		class output_string_adapter : public output_adapter_protocol<CharType>
		{
		public:
			explicit output_string_adapter(std::basic_string<CharType>& s) : str(s) {}

			void write_character(CharType c) override
			{
				str.push_back(c);
			}

			void write_characters(const CharType* s, std::size_t length) override
			{
				str.append(s, length);
			}

		private:
			std::basic_string<CharType>& str;
		};

		template<typename CharType>
		class output_adapter
		{
		public:
			output_adapter(std::vector<CharType>& vec)
				: oa(std::make_shared<output_vector_adapter<CharType>>(vec)) {}

			output_adapter(std::basic_ostream<CharType>& s)
				: oa(std::make_shared<output_stream_adapter<CharType>>(s)) {}

			output_adapter(std::basic_string<CharType>& s)
				: oa(std::make_shared<output_string_adapter<CharType>>(s)) {}

			operator output_adapter_t<CharType>()
			{
				return oa;
			}

		private:
			output_adapter_t<CharType> oa = nullptr;
		};
	}
}

#endif

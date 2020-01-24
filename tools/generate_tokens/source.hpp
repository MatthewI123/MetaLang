#pragma once

#include <cctype>
#include <cstddef>
#include <fstream>
#include <limits>
#include <string>
#include <tuple>
#include <metalang/types.hpp>

class source
{
public:
	explicit source(const char* filename)
	: m_filename(filename), m_file(filename)
	{
		if (!m_file.is_open())
			throw std::runtime_error("could not open source file " + std::string(filename));
	}

	char peek()
	{
		return m_file.peek();
	}

	char get()
	{
		char chr = m_file.get();

		switch (chr) {
			case std::ifstream::traits_type::eof():
				break;
			case '\n':
				m_position += 1;
				m_line += 1;
				m_column = 1;
				break;
			default:
				m_position += 1;
				m_column += 1;
				break;
		}

		return chr;
	}

	char next()
	{
		get();

		return peek();
	}

	auto here() const
	{
		return std::tuple(m_position, m_line, m_column);
	}

	void error(const std::string& message)
	{
		error(message, m_position, m_line, m_column);
	}

	void error(const std::string&message, std::size_t /* m_position */, std::size_t m_line, std::size_t m_column)
	{
		std::cerr << m_filename << ':' << m_line << ':' << m_column << ": " << message << '\n';
		std::exit(1);
	}

private:
	std::string m_filename;
	std::ifstream m_file;
	std::size_t m_position = 1;
	std::size_t m_line = 1;
	std::size_t m_column = 1;
};

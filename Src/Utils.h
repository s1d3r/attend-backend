/*
 * Utils.h
 *
 *  Created on: 13 мая 2023 г.
 *      Author: mr_s
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <sstream>
#include <exception>
#include <boost/stacktrace.hpp>

#ifdef ATTEND_DEBUG
#define CROW_ERROR_RESULT return crow::response{400, exc.what()};
#else
#define CROW_ERROR_RESULT return crow::response{200};
#endif

namespace NFB::Atd {

	std::string makeRequestGet(const std::string &url);
	std::string base64Encode(const std::string &input);
	std::string base64Decode(const std::string &input);
	size_t stringTimeToSeconds(const std::string &str);
	std::string generateUuidV4();

	class Initializer {
	public:
		Initializer();
		~Initializer();
	};

	class ExceptionRuntime : public std::exception {
		std::string Error;

	public:
		ExceptionRuntime() _GLIBCXX_NOTHROW = default;
		ExceptionRuntime(const std::string &file, int line) _GLIBCXX_NOTHROW
			: Error("Ошибочка! Файл: " + file + ", строка " + std::to_string(line) + ":\n")
		{
			  boost::stacktrace::stacktrace st;
			  for(auto iter : st)
			  {
				  std::stringstream stream;
				  stream << iter;
				  Error += '\t' + stream.str() + '\n';
			  }

			  Error += "\t\t";
		}

		~ExceptionRuntime() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW = default;


	    virtual const char* what() const
	    	_GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
			{ return Error.c_str(); }

	    template<typename T>
	    ExceptionRuntime& operator<<(T &&obj)
	    {
	    	if constexpr (std::is_base_of_v<std::string, T>)
	    		Error += obj;
	    	else if constexpr (std::is_constructible_v<std::string, T>)
	    		Error += std::string(std::move(obj));
	    	else
	    		Error += std::to_string(obj);
	    	return *this;
	    }
	};

}



#define ERROR_RN throw NFB::Atd::ExceptionRuntime(__FILE__, __LINE__)

#endif /* UTILS_H_ */

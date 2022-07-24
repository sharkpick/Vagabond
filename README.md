# CppUtilities
CppUtilities contains some useful tools for reading files, parsing XML, managing strings and (eventually) more.

It was inspired by Pete McNeil's CodeDweller library https://code.microneil.com/madscientist/CodeDweller and although it's obviously quite different at this point, eventually this will be as intuitive to use as CodeDweller is.

## file.hpp
file.hpp offers a simple, fast way to read files to memory. can output either a std::string or a std::vector<unsigned char>.

## string.hpp
string.hpp offers some string management functions in the style of Go's string package.
1. string value identification
    * is_uint() attempts to identify std::strings who can be converted to uint64_t
    * is_float() attempts to identify std::strings who can be converted to a long double.
    * is_integer() attempts to identify std::strings who can be converted to int64_t
2. search and sort
    * sort function with overloads for std::string and std::vector<std::string>
3. string formatting
    * split() allows splitting of a string on a delimiter
    * trim_space() trims both leading and trailing space

## xml.hpp and xml.cpp
xml.hpp provides a fairly basic XML parser that attempts to parse XML into Nodes. It will recursively parse other XML objects it finds and embed them as children of the node who contained them. 
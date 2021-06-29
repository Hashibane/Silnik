#include "EditedExceptions.h"
#include <sstream>

Exception::Exception(int linia, const char* plik) noexcept : linia(linia), plik(plik) {}
const char* Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << "\n" << GetOriginString();
	Buffer = oss.str();
	return Buffer.c_str();
}
std::string Exception::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "Plik: " << plik << "\n"<<"Linia: "<< linia;
	return oss.str();
}
const char* Exception::GetType() const noexcept
{
	return "Exception type 1";
}
int Exception::GetLine() const noexcept
{
	return linia;
}
const std::string& Exception::GetFile() const noexcept
{
	return plik;
}
	
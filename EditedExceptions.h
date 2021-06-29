#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
	public:
		Exception(int linia, const char* plik) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;
	private:
		int linia;
		std::string plik;
	protected:
		mutable std::string Buffer;
};
#pragma once
#define NOMINMAX
#include <Windows.h>
#include <commdlg.h>
#include <ShObjIdl.h>
#include <vector>
#include <string>
#include <iostream>

namespace nims_n
{
	struct FileExtensions
	{
		COMDLG_FILTERSPEC csvFilter{ L"CSV", L"*.csv;*.txt;*.dat" };
		COMDLG_FILTERSPEC jpgFilter{ L"JPEG", L"*.jpg;*.jpeg" };
		COMDLG_FILTERSPEC pngFilter{ L"PNG", L"*.png" };
		COMDLG_FILTERSPEC bmpFilter{ L"BMP", L"*.bmp" };
		COMDLG_FILTERSPEC iniFilter{ L"INI", L"*.ini" };
		COMDLG_FILTERSPEC allFilter{ L"ALL", L"*.*" };
	};
	class FileExplorer
	{
	public:
		static std::vector<std::string> openFiles(LPCWSTR title, size_t filterCount, const COMDLG_FILTERSPEC* filters);
		static std::string openFile(LPCWSTR title, size_t filterCount, const COMDLG_FILTERSPEC* filters);
		static std::string openFolder(LPCWSTR title);
	};
}



#include <stdio.h>
#include <windows.h>
#include <debugapi.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

constexpr const wchar_t* DEBUG_ATTACH = L"-debug_attach";
constexpr const wchar_t* DEBUG_ATTACH_2 = L"--debug_attach";

//
// Wait until a debugger attaches and then break the debugger
//
void attach_to_debugger()
{
	while (!IsDebuggerPresent())
	{
		Sleep(1000);
	}

	DebugBreak();
}

int main(int argc, wchar_t* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			auto argument = std::wstring(argv[i]);
			if (argument == DEBUG_ATTACH || argument == DEBUG_ATTACH_2)
			{
				attach_to_debugger();
			}
			else
			{
				std::wcout << L"Unknown arg: \"" << argument << "\"" << std::endl;
				return -1;
			}
		}
	}

	return RUN_ALL_TESTS();
}
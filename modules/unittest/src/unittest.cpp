#include "exo/unittest.hpp"
#include <stdio.h>
#include <stdarg.h>

void ut_execute_tests();

namespace UnitTest {
	static const char* s_currentTestName;
	static int s_numTests;
	static int s_numFailedTests;
	static bool s_currentTestFailed;
	
	void run() {
		s_numTests = 0;
		s_numFailedTests = 0;
		ut_execute_tests();
		printf("%d tests, %d failures\n", s_numTests, s_numFailedTests);
	}

	void execute_test(void (*test)(), const char* name) {
		s_currentTestName = name;
		s_currentTestFailed = false;
		test();
		s_numTests++;
		if(s_currentTestFailed) {
			s_numFailedTests++;
		}
	}
	
	void failed(const char* filename, int line, const char* message, ...) {
		char buffer[4096];
		va_list args;
		va_start(args, message);
		vsprintf(buffer, message, args);
		va_end(args);
		printf("%s failed: %s at %s:%d\n", s_currentTestName, buffer, filename, line);
		s_currentTestFailed = true;
	}
}

#ifndef _UNITTEST_HPP
#define _UNITTEST_HPP

namespace UnitTest {
	void run();

	void execute_test(void (*test)(), const char* name);
	void failed(const char* filename, int line, const char* message, ...);
	
	inline float fabs(float v) { return v < 0 ? -v : v; }

	#define UNIT_TEST(name) void test_##name()
	
	#define UT_ASSERT(cond) if(!cond) UnitTest::failed(__FILE__, __LINE__, "assert(" #cond ")")
	#define UT_ASSERT_EQ(expr, compare) { int ut_r = expr; if(ut_r != compare) UnitTest::failed(__FILE__, __LINE__, #expr " should have been %d but was %d", compare, ut_r); }
	#define UT_ASSERT_EQF(expr, compare, epsilon) { float ut_r = expr; if(UnitTest::fabs(ut_r-compare) > epsilon) UnitTest::failed(__FILE__, __LINE__, #expr " should have been %f but was %f", (float)compare, ut_r); }

	#define UT_ASSERT_EQV2(expr, target) { Vector2 ut_r = expr; if((ut_r-target).getLength() > 0.01f) UnitTest::failed(__FILE__, __LINE__, #expr " should be " #target " but was Vector2(%f, %f)", ut_r.x, ut_r.y); }
	#define UT_ASSERT_EQV3(expr, target) { Vector3 ut_r = expr; if((ut_r-target).getLength() > 0.01f) UnitTest::failed(__FILE__, __LINE__, #expr " should be " #target " but was Vector3(%f, %f, %f)", ut_r.x, ut_r.y, ut_r.z); }
	#define UT_ASSERT_EQV4(expr, target) { Vector4 ut_r = expr; if((ut_r-target).getLength() > 0.01f) UnitTest::failed(__FILE__, __LINE__, #expr " should be " #target " but was Vector4(%f, %f, %f, %f)", ut_r.x, ut_r.y, ut_r.z, ut_r.w); }

}

#endif // _UNITTEST_HPP


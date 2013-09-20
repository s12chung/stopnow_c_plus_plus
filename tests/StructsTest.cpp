/*!
 * This is the test code for the Structs
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestSuite.h>
#include <boost/date_time/posix_time/posix_time.hpp> //include all types plus i/o
#include <Structs.h>
#include <ErrCodes.h>

using namespace boost::posix_time;
using namespace TimerStructs;
using namespace std;

class StructsTest : public CppUnit::TestFixture {
	//macros for test suite code
	CPPUNIT_TEST_SUITE( StructsTest );
	CPPUNIT_TEST( timeStringsTest );
	CPPUNIT_TEST( timeStringValidityPositiveTests );
	CPPUNIT_TEST( timeStringValidityNegativeTests );
	CPPUNIT_TEST_SUITE_END();

private:
	time_duration *t1;time_duration *t2;time_duration *t3;time_duration *t4;
	time_duration *t5;time_duration *t6;time_duration *t7;
public:
	void setUp() {
		t1 = new time_duration(1,2,3,0);
		t2 = new time_duration(10,23,45,0);
		t3 = new time_duration(0,0,0);
		t4 = new time_duration(0,1,0,123456);
		t5 = new time_duration(0,0,0,500000);
		t6 = new time_duration(0,0,0,     5);
		t7 = new time_duration(12,1,12, 500);
	}
	void tearDown() {delete t1; delete t2; delete t3; delete t4; delete t5; delete t6; delete t7;}

	//HELPERS
	void timeStringAsserter(time_duration *t,
			char* full, char* hr, char* min, char* sec) {
		CPPUNIT_ASSERT_EQUAL( toTimeString(*t), string(full) );
		CPPUNIT_ASSERT_EQUAL( toStringHour(*t), string(hr) );
		CPPUNIT_ASSERT_EQUAL( toStringMin(*t), string(min) );
		CPPUNIT_ASSERT_EQUAL( toStringSec(*t), string(sec) );
	}
	void validCheck(time_duration *t) {
		string *str = new string(to_simple_string(*t));
		CPPUNIT_ASSERT( timeStringCheck(str) );
		delete str;
	}
	void invalidCheck(char* str) {
		string *ts = new string(str);
		CPPUNIT_ASSERT_MESSAGE( string("Not Valid! : ")+str, !timeStringCheck(ts) );
		delete ts;
	}

	//TESTS
	//tests toTimeString, toStringHour, toStringMin, toStringSec
	void timeStringsTest() {
		timeStringAsserter(t1, "01:02:03", "01", "02", "03" );
		timeStringAsserter(t2, "10:23:45", "10", "23", "45" );
		timeStringAsserter(t3, "00:00:00", "00", "00", "00" );
	}

	//Checking valid strings for being valid time strings
	void timeStringValidityPositiveTests() {
		validCheck(t1);validCheck(t2);
		validCheck(t3);validCheck(t4);
		validCheck(t5);validCheck(t6);
		validCheck(t7);
	}

	//invalid time string checks
	void timeStringValidityNegativeTests() {
		invalidCheck("a");invalidCheck(":::");invalidCheck(":::.");
		invalidCheck("a:a:a");invalidCheck("aa:aa:aa");
		invalidCheck("1");invalidCheck("10");
		invalidCheck("10:");invalidCheck("10::");invalidCheck("10:10:");
		invalidCheck(":10");invalidCheck("::10");invalidCheck(":10:10");
		invalidCheck("10:10:10.1234567");invalidCheck("10:60:00");
		invalidCheck("10:00:60");invalidCheck("10:10:10.1.");
		invalidCheck("10:10:10.");
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( StructsTest );

//
//int main( int argc, char **argv)
//{
//	CppUnit::TextTestRunner runner;
//	runner.addTest( StructsTest::suite() );
//	runner.run();
//	return 0;
//}


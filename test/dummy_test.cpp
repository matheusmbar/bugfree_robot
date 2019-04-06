#include "CppUTest/TestHarness.h"

TEST_GROUP(dummy_test){
   
};

TEST (dummy_test, pass_me){
	CHECK_EQUAL(1,1);
}

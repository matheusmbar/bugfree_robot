#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "multiply.h"

TEST_GROUP(multiply_funtion){
   
};

TEST (multiply_funtion, two_positives){
	CHECK_EQUAL( 2, multiply(  1, 2));
	CHECK_EQUAL(18, multiply(  3, 6));
	CHECK_EQUAL( 0, multiply(  0, 3));
	CHECK_EQUAL( 0, multiply(100, 0));
}

TEST(multiply_funtion, two_negatives){
	CHECK_EQUAL( 15, multiply(-5,-3));
}

TEST(multiply_funtion, opposite_signs){
	CHECK_EQUAL(-20, multiply(-10,2));
}

TEST(multiply_funtion, max_min_values){
	CHECK_EQUAL( 16129, multiply( 127, 127));
	CHECK_EQUAL( 16384, multiply(-128,-128));
	CHECK_EQUAL(-16256, multiply( 127,-128));
}
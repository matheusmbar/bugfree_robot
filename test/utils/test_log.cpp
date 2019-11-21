#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport_c.h>

extern "C"{
    #include "bugfree/utils/log/log.h"
}
#include <main.h>

retcode_t mock_print (const char* msg, uint16_t msg_len){
    // printf ("\nmsg_len: %d\n%s\n", msg_len, msg);
    return (retcode_t) mock_c()->actualCall("mock_print")
        ->withMemoryBufferParameter("msg", (const unsigned char*)msg, msg_len)
        ->withUnsignedIntParameters("msg_len", msg_len)
        ->returnUnsignedIntValueOrDefault(RET_OK);
}

TEST_GROUP(log){
    loglevel_t level;
    retcode_t ret;
    void setup(){

    }

    void teardown (){
        mock_c()->checkExpectations();
        mock_c()->clear();
    }
};

TEST (log, init){
    ret = log_init(LOG_DEBUG);
    level = log_getLevel();
    CHECK_EQUAL(RET_OK, ret);
    CHECK_EQUAL(LOG_DEBUG, level);

    ret = log_init(LOG_ERROR);
    level = log_getLevel();
    CHECK_EQUAL(RET_OK, ret);
    CHECK_EQUAL(LOG_ERROR, level);
}

TEST (log, set_get_level){
    ret = log_setLevel(LOG_INFO);
    level = log_getLevel();
    CHECK_EQUAL(RET_OK, ret);
    CHECK_EQUAL(LOG_INFO, level);

    ret = log_setLevel(LOG_ERROR);
    level = log_getLevel();
    CHECK_EQUAL(RET_OK, ret);
    CHECK_EQUAL(LOG_ERROR, level);
}

TEST (log, set_get_callback){
    ret = log_setCallback (mock_print);
    CHECK_EQUAL(RET_OK, ret);
    FUNCTIONPOINTERS_EQUAL (mock_print, log_getCallback());

    //NULL pointer returns error amd won't modify callback function pointer
    ret = log_setCallback (NULL);
    CHECK_EQUAL(RET_ERROR, ret);
    FUNCTIONPOINTERS_EQUAL (mock_print, log_getCallback());
}

TEST (log, init_resets_callback){
    log_setCallback (mock_print);
    FUNCTIONPOINTERS_EQUAL (mock_print, log_getCallback());
    
    log_init(LOG_DEBUG);
    FUNCTIONPOINTERS_EQUAL (NULL, log_getCallback());
}


TEST_GROUP(log_mock_print){
    retcode_t ret;
    void setup(){

    }

    void teardown (){
        mock_c()->checkExpectations();
        mock_c()->clear();
    }
};

TEST (log_mock_print, mock_print){
    //return parameter is RET_OK by default
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "mock test 1", 11)
    ->withUnsignedIntParameters("msg_len", 11);
    ret = mock_print("mock test 1", 11);
    CHECK_EQUAL(RET_OK, ret);

    //return parameter may be specified in mock expect call
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "mock test 02", 12)
    ->withUnsignedIntParameters("msg_len", 12)
    ->andReturnUnsignedIntValue(RET_OK);
    ret = mock_print("mock test 02", 12);
    CHECK_EQUAL(RET_OK, ret);

    //return parameter may be specified in mock expect call
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "mock test 003", 13)
    ->withUnsignedIntParameters("msg_len", 13)
    ->andReturnUnsignedIntValue(RET_ERROR);
    ret = mock_print("mock test 003", 13);
    CHECK_EQUAL(RET_ERROR, ret);
}


TEST_GROUP(log_with_init){
    loglevel_t level;
    retcode_t ret;
    void setup(){
        log_init(LOG_DEBUG);
        log_setCallback (mock_print);
    }

    void teardown (){
        mock_c()->checkExpectations();
        mock_c()->clear();
    }
};

TEST (log_with_init, simple_prints){

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "Hello world", 11)
    ->withUnsignedIntParameters("msg_len", 11);
    ret = log_print (LOG_DEBUG, (char*) "Hello world");
    CHECK_EQUAL (RET_OK, ret);

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "Testing callback 12345", 22)
    ->withUnsignedIntParameters("msg_len", 22);
    log_print (LOG_DEBUG, (char*) "Testing callback %d", 12345);

    char msg[] = "from char array";
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "from char array", 15)
    ->withUnsignedIntParameters("msg_len", 15);
    log_print (LOG_DEBUG, (char*) "%s", msg);

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "1 2 3 5.50", 10)
    ->withUnsignedIntParameters("msg_len", 10);
    log_print (LOG_DEBUG, (char*) "%d %d %d %.2f", 1,2,3,5.5);
}


TEST (log_with_init, print_priority_check){
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "Hello world", 11)
    ->withUnsignedIntParameters("msg_len", 11);
    log_print (LOG_INFO, (char*) "Hello world");

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "HELLO world", 11)
    ->withUnsignedIntParameters("msg_len", 11);
    log_print (LOG_WARNING, (char*) "HELLO world");

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "Hello WORLD", 11)
    ->withUnsignedIntParameters("msg_len", 11);
    log_print (LOG_CRITICAL, (char*) "Hello WORLD");

    //set log level to print only messages with priority higher or equal to WARNING
    log_setLevel(LOG_WARNING);

    ret = log_print (LOG_DEBUG, (char*) "Hello world");
    CHECK_EQUAL (RET_WRONG_PARAM, ret);
    ret = log_print (LOG_INFO, (char*) "HELLO world");
    CHECK_EQUAL (RET_WRONG_PARAM, ret);

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "HELLO WORLD", 11)
    ->withUnsignedIntParameters("msg_len", 11);
    ret = log_print (LOG_WARNING, (char*) "HELLO WORLD");
    CHECK_EQUAL (RET_OK, ret);

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "Hello WORLD", 11)
    ->withUnsignedIntParameters("msg_len", 11);
    ret = log_print (LOG_CRITICAL, (char*) "Hello WORLD");
    CHECK_EQUAL (RET_OK, ret);

    //set level to CRITICAL
    log_setLevel(LOG_CRITICAL);
    log_print (LOG_DEBUG, (char*) "Hello world");
    log_print (LOG_INFO, (char*) "Hello world");
    log_print (LOG_WARNING, (char*) "Hello world");
    log_print (LOG_ERROR, (char*) "Hello world");

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "Hello CRITICAL", 14)
    ->withUnsignedIntParameters("msg_len", 14);
    log_print (LOG_CRITICAL, (char*) "Hello CRITICAL");
}

TEST (log_with_init, invalid_callback_function){
    log_init(LOG_DEBUG);

    mock_c()->expectNoCall("mock_print");
    log_print (LOG_DEBUG, (char*) "Hello world");
    log_print (LOG_INFO, (char*) "Hello world");
    log_print (LOG_WARNING, (char*) "Hello world");
    log_print (LOG_ERROR, (char*) "Hello world");
    log_print (LOG_CRITICAL, (char*) "Hello world");
}

TEST (log_with_init, callback_function_returns_error){
    //log_print function will return RET_RESOURCE_UNAVAILABLE if 
    //callback funtion resturns an error code

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "Hello world", 11)
    ->withUnsignedIntParameters("msg_len", 11)
    ->andReturnUnsignedIntValue(RET_ERROR);

    ret = log_print (LOG_DEBUG, (char*) "Hello world");
    CHECK_EQUAL (RET_RESOURCE_UNAVAILABLE, ret);


    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "HELLO WORLD", 11)
    ->withUnsignedIntParameters("msg_len", 11)
    ->andReturnUnsignedIntValue(RET_WRONG_PARAM);

    ret = log_print (LOG_DEBUG, (char*) "HELLO WORLD");
    CHECK_EQUAL (RET_RESOURCE_UNAVAILABLE, ret);
}


TEST (log_with_init, default_max_msg_len){
    // default max msg len = 100

    //works with 100 characters
    char msg_100[] = "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789";
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789", 100)
    ->withUnsignedIntParameters("msg_len", 100);

    log_print (LOG_DEBUG, "%s", msg_100);


    //will crop what is bigger than 100 characters
    char msg_101[] = "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_";

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789", 100)
    ->withUnsignedIntParameters("msg_len", 100);

    log_print (LOG_DEBUG, "%s", msg_101);


    char msg_110[] = "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789";
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789", 100)
    ->withUnsignedIntParameters("msg_len", 100);

    log_print (LOG_DEBUG, "%s", msg_110);
}

TEST (log_with_init, init_resets_default_len){
    log_setMaxLen (20);
    log_init(LOG_DEBUG);
    log_setCallback (mock_print);

    char msg_100[] = "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789";
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789", 100)
    ->withUnsignedIntParameters("msg_len", 100);

    log_print (LOG_DEBUG, "%s", msg_100);

}

TEST (log_with_init, set_max_msg_len){

    log_setMaxLen (20);

    //works with 100 characters
    char msg_20[] = "_123456789_123456789";
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "_123456789_123456789_", 20)
    ->withUnsignedIntParameters("msg_len", 20);

    log_print (LOG_DEBUG, "%s", msg_20);


    //will crop what is bigger than 20 characters
    char msg_21[] = "_123456789_123456789_";

    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "_123456789_123456789_", 20)
    ->withUnsignedIntParameters("msg_len", 20);

    log_print (LOG_DEBUG, "%s", msg_21);


    //will allow msgs bigger than default max len
    log_setMaxLen (105);
    char msg_110[] = "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789";
    mock_c()->expectOneCall("mock_print")
    ->withMemoryBufferParameter("msg", (const unsigned char*) "_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_1234", 105)
    ->withUnsignedIntParameters("msg_len", 105);

    log_print (LOG_DEBUG, "%s", msg_110);
}

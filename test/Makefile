# This is the test makefile for bugfree-robot firmware
#
# It works based on CppUTest

## Variables to help building absolute paths

MAKEFILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

PROJECT_DIR=$(realpath $(MAKEFILE_DIR)..)

TEST_DIR=$(PROJECT_DIR)/test


CPPUTEST_HOME=$(TEST_DIR)/cpputest


# -------------------- Source files paths ----------------------- 
# project source files path
SRC_DIRS=$(PROJECT_DIR)
SRC_DIRS+=$(PROJECT_DIR)/bugfree/drivers/io
SRC_DIRS+=$(PROJECT_DIR)/bugfree/utils/log

# project header files path
INCLUDE_DIRS=$(PROJECT_DIR)
INCLUDE_DIRS+=$(PROJECT_DIR)/Inc
INCLUDE_DIRS+=$(PROJECT_DIR)/Drivers/STM32F3xx_HAL_Driver/Inc
INCLUDE_DIRS+=$(PROJECT_DIR)/Drivers/CMSIS/Device/ST/STM32F3xx/Include
INCLUDE_DIRS+=$(PROJECT_DIR)/Drivers/CMSIS/Include


# add this to avoid problems when including cpputest headers on test files
INCLUDE_DIRS+=$(CPPUTEST_HOME)/include

## test source files path
TEST_SRC_DIRS=$(TEST_DIR)
TEST_SRC_DIRS+=$(TEST_DIR)/drivers
TEST_SRC_DIRS+=$(TEST_DIR)/utils
TEST_SRC_DIRS+=$(TEST_DIR)/mocks_tests

## mock path
MOCKS_SRC_DIRS=$(TEST_DIR)/mocks
MOCKS_SRC_DIRS+=$(TEST_DIR)/mocks/drivers
MOCKS_SRC_DIRS+=$(TEST_DIR)/mocks/utils

#---------------------- Build flags ------------------
CPPUTEST_CPPFLAGS+=-DSTM32F302xC
CPPUTEST_CPPFLAGS+=-Wno-error=unused-parameter

#build for 32bits architecture
CPPUTEST_CPPFLAGS+=-mx32
CPPUTEST_LDFLAGS+=-mx32

## what to call the test binary
TEST_TARGET=test_runner

# Add the library linking to your Makefile
LD_LIBRARIES += -L$(CPPUTEST_HOME)/lib -lCppUTestExt

#Reduce the excess of prints in the build process
ifndef SILENCE
	SILENCE = @
endif

#run MakefileWorker.mk with the variables defined here
include $(CPPUTEST_HOME)/build/MakefileWorker.mk


.PHONY: run
run: start $(TEST_TARGET)
	$(RUN_TEST_TARGET)

.PHONY: check_paths
check_paths:
		@echo "\nRepository dir\t" $(REPOSITORY_DIR)
		@echo "\nTest dir\t" $(TEST_DIR)
		@echo "\nMakefile dir\t" $(MAKEFILE_DIR)
		@echo "\nProject dir\t" $(PROJECT_DIR)
		@echo "\nSrc dir  \t" $(SRC_DIR)
		@echo "\nSrc dirs  \t" $(SRC_DIRS)
		@echo "\nInclude dirs  \t" $(INCLUDE_DIRS)
		@echo "\nCpputest home  \t" $(CPPUTEST_HOME)

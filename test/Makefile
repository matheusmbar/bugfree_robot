# This is the test makefile for bugfree-robot firmware
#
# It works based on CppUTest


## we don't want to use relative paths, so we set these variables
MAKEFILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

# ## Define project directory path
REPOSITORY_DIR=$(realpath $(MAKEFILE_DIR)..)

PROJECT_DIR=$(REPOSITORY_DIR)

## test folder directory
TEST_DIR=$(PROJECT_DIR)/test

#where the cpputest library is located
CPPUTEST_HOME=$(TEST_DIR)/cpputest


# -------------------- Source files paths ----------------------- 
# project source files path
SRC_DIRS=$(PROJECT_DIR)

# project header files path
INCLUDE_DIRS=$(PROJECT_DIR)
# add this to avoid problems when including cpputest headers on test files
INCLUDE_DIRS+=$(CPPUTEST_HOME)/include

## specify where the test code is located
TEST_SRC_DIRS=$(TEST_DIR)

## what to call the test binary
TEST_TARGET=test_runner

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
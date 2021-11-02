#!/bin/bash

# Execute this script typing
#    bash compule_cpputest.sh
# It will execute the three steps needed to get cpputest ready to build the repository tests
# This must be executed every time the repository is cloned


#commands to add cpputest as a submodule on this repository
#mkdir test
#git submodule add https://github.com/cpputest/cpputest test/cpputest/

#commands to compile cpputest
cd cpputest
autoreconf -fi
./autogen.sh
#./configure
./configure "CFLAGS=-mx32" "CXXFLAGS=-mx32" "LDFLAGS=-mx32"
make -j4
cd ..

echo -e "\n\nCpputest build finished\n\n"

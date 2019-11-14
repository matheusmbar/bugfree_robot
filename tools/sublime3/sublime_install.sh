#!/bin/bash

echo -e "Copying Sublime text editor config files to user folder:"
echo -e $(realpath ~/.config/sublime-text-3/Packages/User/)

cp *.sublime* ~/.config/sublime-text-3/Packages/User/

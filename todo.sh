#!/bin/sh
# Bash to-do list

#Copy necessary files
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cp -a $DIR/. ~
rm -r ~/.git

# Vim settings
## First, download the Vundle library
git clone https://github.com/gmarik/vundle.git ~/.vim/bundle/vundle
## Install plugins
vim -c ":PluginInstall"


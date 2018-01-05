#!/bin/sh
# Bash to-do list

# Copy necessary files
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cp -a $DIR/. ~
rm -rf ~/.git ~/todo.sh ~/todocat.sh ~/wakatime-key

# Vim settings
## First, download the Vundle library if haven't already
if [[ ! -d  ~/.vim/bundle/vundle ]] ; then git clone https://github.com/gmarik/vundle.git ~/.vim/bundle/vundle ; fi
## Install plugins
vim -c ":PluginInstall"


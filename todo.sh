#!/bin/sh
# Bash to-do list

#Copy necessary files
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cp -a $DIR/. ~

# Vim settings
## First, download the Vundle library
git clone https://github.com/gmarik/vundle.git ~/.vim/bundle/vundle
vim -c ":PluginInstall"
echo "lol"
exit 1

# Fun with cat facts -- Ubuntu only
## Disable all other MOTDs and backup the 00-header
sudo chmod -x /etc/update-motd.d/*
sudo mv /etc/update-motd.d/00-header /etc/update-motd.d/000-header
## Create new 00-header with cat facts
echo "#!/bin/sh" > 00-header
echo "echo \"You have come far. Can I interest you with a fact about cats?\"" >> 00-header
echo "curl -sX GET --header 'Accept: application/json' 'https://catfact.ninja/fact' \
| python3 -c \"import sys, json; print(json.load(sys.stdin)['fact']);\"" >> 00-header
chmod +x 00-header
sudo mv 00-header /etc/update-motd.d/

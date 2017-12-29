# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions

# Ctrl+S doesn't freeze terminal
stty -ixon
stty susp undef

# Press Ctrl+S/Alt+S to run last command with sudo
bind '"\C-s":"sudo !!\n"'
bind '"\es":"sudo !!\n"'

# Run vim with as many config as it is possible as sudo
alias svim='sudo vim -S ~/.vimrc_root'

# Set prompt
export PS1="\[\033[92m\]\u@\h \[\033[33m\]\W\[\033[96m\]\$(command -v git &>/dev/null && git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/ (\1)/')\[\033[00m\] \\$ "
# Set old working directory (cd -)
export OLDPWD=$( cat ~/.oldpwd 2>/dev/null )

PATH=$PATH:$HOME/bin

export PATH


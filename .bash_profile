# .bash_profile

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi

# User specific environment and startup programs

# Ctrl+S doesn't freeze terminal
stty -ixon

# Press Ctrl+S/Alt+S to run last command with sudo
bind '"\C-s":"sudo !!\n"'
bind '"\es":"sudo !!\n"'

PATH=$PATH:$HOME/bin

export PATH

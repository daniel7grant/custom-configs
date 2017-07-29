# .bash_profile

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi

# User specific environment and startup programs

# Press Alt+S to run last command with sudo
bind '"\es":"sudo !!\n"'

PATH=$PATH:$HOME/bin

export PATH

# .bash_profile

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi

# User specific environment and startup programs

# Ctrl+S doesn't freeze terminal
stty -ixon
stty susp undef

# Press Ctrl+S/Alt+S to run last command with sudo
bind '"\C-s":"sudo !!\n"'
bind '"\es":"sudo !!\n"'

# Run vim with as many config as it is possible as sudo
alias svim='sudo vim -S ~/.vimrc_root'

parse_git_branch() {
	git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/ (\1)/'
}

export PS1="[\u@\h \W\$(parse_git_branch)]$ "
# Uncomment for colorful magic
#export PS1="\[\033[92m\]\u@\h \[\033[33m\]\W\[\033[96m\]\$(parse_git_branch)\[\033[00m\] $ "

#PS1="[\u@\h \W$( git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/ (\1)/' )]\$ "

PATH=$PATH:$HOME/bin

export PATH

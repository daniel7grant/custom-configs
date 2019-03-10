# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions

# If shell is not interactive quit
if [ -z "$PS1" ]; then
	return
fi

# Ctrl+S doesn't freeze terminal
stty -ixon
stty susp 

# Tabwidth 4
tabs 4

# Allow for better history
shopt -s histappend
export HISTFILESIZE=1000000
export HISTSIZE=1000000
export HISTCONTROL=ignoreboth
export HISTIGNORE='bg:fg:history'

# Press Ctrl+S/Alt+S to run last command with sudo
bind '"\C-s":"\C-e\C-usudo !!\n"'
bind '"\es":"\C-e\C-usudo !!\n"'
# Press Alt+C to comment out current line
bind '"\ec":"\e#"'

# Run vim with as many config as it is possible as sudo
alias svim='sudo vim -pS ~/.vimrc_root'
# I like colors
alias ls='ls --color -h'
alias ll='ls --color -hl'
alias grep='grep --color=always'
alias jest='jest --color'
# And tabs
alias vim='vim -p'
# Stfu curl
alias curl='curl -s'

getbranchinfo(){
	if [[ -n $(git rev-parse --is-inside-work-tree 2>/dev/null) ]]
	then
		BRANCH=$(git rev-parse --abbrev-ref HEAD 2>/dev/null)
		if [[ -n $(git status -s) ]] ; then MOD='*' ; fi
		REF=$(git for-each-ref --format="%(refname:short) %(upstream:track) " refs/heads 2>/dev/null | grep -E "^$BRANCH" -m1 --color=never \
			| sed -e "s/$BRANCH/$BRANCH$MOD/g" -e 's/ahead /↑/g' -e 's/behind /↓/g' | tr -d '[, ]')
		[[ -n $REF ]] && echo "($REF) " || echo "($BRANCH$MOD) "
	fi
}

# Set prompt
export PS1="\[\033[92m\]\u@\h \[\033[33m\]\W\[\033[96m\] \$( getbranchinfo 2>/dev/null )\[\033[00m\]\\$ "
# Set old working directory (cd -)
export OLDPWD=$( cat ~/.oldpwd 2>/dev/null )
# Set favourite editor
export EDITOR="vim -p"

PATH=$PATH:$HOME/bin

export PATH


# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions

# Ctrl+S doesn't freeze terminal
stty -ixon
stty susp 

# Press Ctrl+S/Alt+S to run last command with sudo
bind '"\C-s":"sudo !!\n"'
bind '"\es":"sudo !!\n"'

# Run vim with as many config as it is possible as sudo
alias svim='sudo vim -S ~/.vimrc_root'
# I like colors
alias ls='ls --color -h'

getbranchinfo(){
	if [[ -n $(git rev-parse --is-inside-work-tree 2>/dev/null) ]]
	then
		BRANCH=$(git rev-parse --abbrev-ref HEAD 2>/dev/null)
		if [[ -n $(git status -s) ]] ; then MOD='*' ; fi
		REF=$(git for-each-ref --format="%(refname:short) %(upstream:track) " refs/heads 2>/dev/null | grep -E "^$BRANCH" -m1 \
			| sed -e "s/$BRANCH/$BRANCH$MOD/g" -e 's/ahead /↑/g' -e 's/behind /↓/g' | tr -d '[, ]')
		[[ -n $REF ]] && echo "($REF) " || echo "($BRANCH$MOD) "
	fi
}

# Set prompt
export PS1="\[\033[92m\]\u@\h \[\033[33m\]\W\[\033[96m\] \$( getbranchinfo )\[\033[00m\]\\$ "
# Set old working directory (cd -)
export OLDPWD=$( cat ~/.oldpwd 2>/dev/null )
# Set favourite editor
export EDITOR="vim -p"

PATH=$PATH:$HOME/bin

export PATH


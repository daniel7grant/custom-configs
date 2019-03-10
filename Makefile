bashfiles = .ssh/ .bashrc .bash_logout .inputrc .gitconfig .gitattributes
vimfiles = .vim/ .vimrc .vimrc_root

default: bash-install vim-install
bash-install:
	cp -ar $(bashfiles) ~ 
vim-install:
	cp -ar $(vimfiles) ~ 
	if [[ ! -d  ~/.vim/bundle/vundle ]] ; then git clone https://github.com/gmarik/vundle.git ~/.vim/bundle/vundle ; fi
	vim wakatime-key -c ":PluginInstall"

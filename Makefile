bashfiles = .config/ .ssh/ .bashrc .bash_logout .inputrc .gitconfig .gitattributes
privatefiles = private/.ssh/ 
vimfiles = .vim/ .vimrc .vimrc_root

default: bash-install vim-install
encrypt: 
	export GPG_TTY=`tty`
	if [[ -d private ]] ; then tar cz private/ | gpg --yes --cipher-algo AES256 -co private.tar.gz.gpg - ; fi
decrypt: 
	export GPG_TTY=`tty`
	gpg -d private.tar.gz.gpg | tar xz
bash-install:
	cp -ar $(bashfiles) ~ 
	if [[ -d private ]] ; then cp -ar $(privatefiles) ~ ; fi
vim-install:
	cp -ar $(vimfiles) ~ 
	# if [[ ! -d  ~/.vim/bundle/vundle ]] ; then git clone https://github.com/gmarik/vundle.git ~/.vim/bundle/vundle ; fi
	vim wakatime-key -c ":PlugInstall"

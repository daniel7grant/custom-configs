set nocompatible
filetype off
set rtp+=~/.vim/bundle/vundle/
call vundle#rc()

"This is the Vundle package, which can be found on GitHub.
" For GitHub repos, you specify plugins using the
" 'user/repository' format
Plugin 'gmarik/vundle'
Plugin 'wakatime/vim-wakatime'

filetype plugin indent on
set tabstop=4
set number
set showmatch
set hlsearch
set incsearch
set ignorecase
set smartcase
set backspace=2
set autoindent
set formatoptions=q,r
set ruler
set background=dark
filetype detect
filetype plugin on
filetype indent on
syntax on
set nocompatible
set foldmethod=syntax

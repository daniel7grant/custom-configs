set nocompatible  
set rtp+=~/.vim/bundle/vundle/
call vundle#rc()

"This is the Vundle package, which can be found on GitHub.
" For GitHub repos, you specify plugins using the
" 'user/repository' format
Plugin 'gmarik/vundle'
Plugin 'wakatime/vim-wakatime'

" SET COLORS
"colorscheme badwolf 
set background=dark

" INDENTATION SETTINGS
filetype plugin indent on
set cindent
set tabstop=4
set softtabstop=4
set shiftwidth=4

" SET LINE NUMBERS (SWITCH-OFF: SET NONUMBER)
set number

" SHOW LAST COMMAND
set showcmd

" SHOW AUTOCOMPLETE MENU FOR COMMANDS
set wildmenu

" PERFORMANCE OPTIMIZATION
set lazyredraw
set nocompatible

" SHOW MATCHING PARENTHESES
set showmatch

" SET BACKSPACE MAPPING
set backspace=2

" SEARCH SETTINGS
set hlsearch
set incsearch
set ignorecase
set smartcase

" CONTROL FOLDING
set foldenable
set foldlevelstart=10
set foldnestmax=15
set foldmethod=syntax

" DISABLE CONTINUING COMMENT ON <ENTER>
autocmd FileType * setlocal formatoptions-=c formatoptions-=r formatoptions-=o

" SET COMMON KEYBOARD COMBINATIONS
map <C-a> <Esc>ggVG<CR>
map <C-b> %
map <C-d> yyp$a
imap <C-d> <Esc>yyp$a
map <C-f> /
imap <C-f> <Esc>/
map <C-h> :%s/
imap <C-h> <Esc>:%s/
map <C-k> ddkP
imap <C-k> <Esc>ddkPa
map <C-l> ddp
imap <C-l> <Esc>ddpa
map <C-s> :w<CR>
imap <C-s> <Esc>:w<CR>
map <C-y> <Esc>dd






set nocompatible  
set rtp+=~/.vim/bundle/vundle/
call vundle#rc()

"This is the Vundle package, which can be found on GitHub.
" For GitHub repos, you specify plugins using the
" 'user/repository' format
Plugin 'gmarik/vundle'
Plugin 'wakatime/vim-wakatime'
Plugin 'vim-airline/vim-airline'
Plugin 'vim-airline/vim-airline-themes'
Plugin 'tpope/vim-fugitive'

" SET COLORS
syntax on
set t_Co=256
set background=dark
let g:badwolf_darkgutter = 1
let g:badwolf_css_props_highlight = 1
let g:jellybeans_use_lowcolor_black = 1
let g:rehash256 = 1
let g:sierra_Campfire = 1
colorscheme molokai

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

" PERSISTENT UNDO
set undofile
set undodir=$HOME/.vim/undo
set undolevels=1000
set undolevels=1000

" DISABLE CONTINUING COMMENT ON <ENTER>
autocmd FileType * setlocal formatoptions-=c formatoptions-=r formatoptions-=o

" SET DIRECTORY TO FILE'S POSITION
autocmd BufEnter * silent! lcd %:p:h

" SET COMMON KEYBOARD COMBINATIONS
"" Jump to other brace
map <C-b> %
"" Duplicate line
map <C-d> yyp$a
imap <C-d> <Esc>yyp$a
vmap <C-d> yP
"" Find
map <C-f> /
imap <C-f> <Esc>/
"" Replace
map <C-h> :%s/
imap <C-h> <Esc>:%s/
"" Move lines down or up
map <C-j> ddp
imap <C-j> <Esc>ddpa
map <C-k> ddkP
imap <C-k> <Esc>ddkPa
"" Remove line
map <C-l> <Esc>dd
"" New file 
map <C-n> :tabnew<CR>
imap <C-n> <Esc>:tabnew<CR>
"" Open current folder in new tab
map <C-o> :tabe .<CR>
imap <C-o> <Esc>:tabe .<CR>
"" Save, save&quit
map <C-s> :w<CR>
imap <C-s> <Esc>:w<CR>
map <C-q> :wq<CR>
imap <C-q> <Esc>:wq<CR>
"" Redo, undo
map <C-y> <C-r>
imap <C-y> <Esc><C-r>
map <C-z> u
imap <C-z> <Esc>u
" Tabs tabs tabs
map <C-t> gt
imap <C-t> <Esc>gti
map t :tabedit<Space>
map t<Space> :tabedit<Space>
map t :tabedit 
map t1 1gt
map t2 2gt
map t3 3gt
map t4 4gt
map t5 5gt
map t6 6gt
map t7 7gt
map t8 8gt
map t9 9gt
" Fugitive commands
map ga <Esc>:Gstatus<CR>
map gs <Esc>:Gstatus<CR>
map gc <Esc>:Gcommit<CR>
map gp <Esc>:Gpush<CR>
map gr <Esc>:Gpull<CR>

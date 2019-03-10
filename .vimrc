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
autocmd Filetype yaml setlocal ts=2 sts=2 sw=2 expandtab

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

" SET COMMON COMMANDS
command W w !sudo tee % > /dev/null

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
vmap <C-h> :s/
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
noremap <C-t> gt
inoremap <C-t> <Esc>gti
noremap t :tabedit<Space>
noremap t<Space> :tabedit<Space>
noremap t :tabedit 
noremap t1 1gt
noremap t2 2gt
noremap t3 3gt
noremap t4 4gt
noremap t5 5gt
noremap t6 6gt
noremap t7 7gt
noremap t8 8gt
noremap t9 9gt
" Fugitive commands
map ga <Esc>:Gstatus<CR>
map gs <Esc>:Gstatus<CR>
map gc <Esc>:Gcommit<CR>
map gp <Esc>:Gpush<CR>
map gr <Esc>:Gpull<CR>

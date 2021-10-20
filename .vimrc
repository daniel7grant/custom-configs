set nocompatible  

" Move to Plug
call plug#begin('~/.vim/plugged')

" Plug plugins
" For GitHub repos, you specify plugins using the
" 'user/repository' format
Plug 'wakatime/vim-wakatime'
Plug 'mbbill/undotree'
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
Plug 'preservim/nerdcommenter'
Plug 'tpope/vim-surround'
" NERDTREE
Plug 'preservim/nerdtree'
Plug 'tiagofumo/vim-nerdtree-syntax-highlight'
Plug 'Xuyuanp/nerdtree-git-plugin' 
Plug 'ryanoasis/vim-devicons'
Plug 'jistr/vim-nerdtree-tabs'
Plug 'junegunn/fzf.vim'
" AUTOCOMPLETE, LINT, FORMAT
Plug 'mattn/emmet-vim'
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'w0rp/ale'
Plug 'prettier/vim-prettier'
" FILETYPES
Plug 'chr4/nginx.vim'
Plug 'cespare/vim-toml'
Plug 'jelera/vim-javascript-syntax'
Plug 'othree/javascript-libraries-syntax.vim'
Plug 'nfnty/vim-nftables'
Plug 'posva/vim-vue'
Plug 'pearofducks/ansible-vim'

call plug#end()

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

" SET LEADER
let mapleader = ","

" SET AUTOCOMPLETE SETTINGS
let g:ale_fixers = {'*': ['remove_trailing_lines', 'trim_whitespace'], 'javascript': ['prettier', 'eslint']}
let g:coc_user_config = {}
let g:coc_global_extensions = [
  \ 'coc-emmet', 
  \ 'coc-css', 
  \ 'coc-html', 
  \ 'coc-json', 
  \ 'coc-prettier', 
  \ 'coc-tsserver', 
  \ 'coc-snippets', 
  \ 'coc-lua',
  \ 'coc-python',
  \ 'coc-java',
  \ 'coc-rls',
  \ 'coc-rust-analyzer',
  \ 'coc-yaml',
  \ 'coc-eslint']
let g:coc_disable_startup_warning = 1

" NERDTREE SETTINGS
let g:NERDTreeShowHidden = 1
let g:NERDTreeMinimalUI = 1 " hide helper
let g:NERDTreeIgnore = ['^.git$', '^node_modules$', '^vendor$'] " ignore node_modules to increase load speed
let g:NERDTreeStatusline = '' " set to empty to use lightline
noremap <C-b> :NERDTreeToggle<CR>

" FUZZY FIND SETTINGS
nnoremap <C-p> :GFiles<CR>
let g:fzf_action = {
  \ 'ctrl-t': 'tab split',
  \ 'ctrl-s': 'split',
  \ 'ctrl-v': 'vsplit'}

" INDENTATION SETTINGS
filetype plugin indent on
set cindent
set expandtab
set tabstop=4
set softtabstop=4
set shiftwidth=4
autocmd Filetype yaml setlocal ts=2 sts=2 sw=2 expandtab

" FILETYPES
autocmd BufRead,BufNewFile */playbooks/*.yml set filetype=yaml.ansible

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
nnoremap <C-u> :UndotreeToggle<CR>

" COMMENT OUT BLOCKS AND LINES
nmap Kk <Plug>NERDCommenterComment
vmap Kk <Plug>NERDCommenterComment
nmap KK <Plug>NERDCommenterToggle
vmap KK <Plug>NERDCommenterToggle<CR>gv

" DISABLE CONTINUING COMMENT ON <ENTER>
autocmd FileType * setlocal formatoptions-=c formatoptions-=r formatoptions-=o

" SET DIRECTORY TO FILE'S POSITION
autocmd BufEnter * silent! lcd %:p:h

" SET COMMON COMMANDS
command W w !sudo tee % > /dev/null

" SET COMMON KEYBOARD COMBINATIONS
"" Duplicate line
noremap <C-d> yyp$a
inoremap <C-d> <Esc>yyp$a
vnoremap <C-d> yP
"" Find
noremap <C-f> /
inoremap <C-f> <Esc>/
"" Replace
noremap <C-h> :%s/
inoremap <C-h> <Esc>:%s/
vnoremap <C-h> :s/
"" Move lines down or up
noremap <C-j> ddp
inoremap <C-j> <Esc>ddpa
noremap <C-k> ddkP
inoremap <C-k> <Esc>ddkPa
"" Remove line
noremap <C-l> <Esc>dd
"" New file 
noremap <C-n> :tabnew<CR>
inoremap <C-n> <Esc>:tabnew<CR>
"" Open current folder in new tab
noremap <C-o> :tabe .<CR>
inoremap <C-o> <Esc>:tabe .<CR>
"" Save, save&quit
noremap <C-s> :w<CR>
inoremap <C-s> <Esc>:w<CR>
noremap <C-q> :wq<CR>
inoremap <C-q> <Esc>:wq<CR>
"" Redo, undo
noremap <C-y> <C-r>
inoremap <C-y> <Esc><C-r>
noremap <C-z> u
inoremap <C-z> <Esc>u
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
noremap ga <Esc>:Gstatus<CR>
noremap gs <Esc>:Gstatus<CR>
noremap gc <Esc>:Gcommit<CR>
noremap gp <Esc>:Gpush<CR>
noremap gr <Esc>:Gpull<CR>



:set path+=,$PWD/core/include
:set path+=,$PWD/core/src
:set path+=,$PWD/test/src

" vertical split window
:vsplit

" set windows width as the same
:wincmd =

" increase left window width
:30wincmd >

" edit .c files in the left window
:edit $PWD/core/src/lexer.c
:edit $PWD/core/src/parser.c

" Edit .h files on the right window
:wincmd l
:edit $PWD/core/src/lexer.h
:edit $PWD/core/src/parser.h

" back to left window
:wincmd h



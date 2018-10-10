#!/usr/bin/env zsh
IFS="
"
for file in `find . -name "*.s"`; do filename=`echo $file | grep -oE "\./[^.]+"`; ./asm "$filename.s" 2>/dev/null 1>/dev/null ; hexdump "$filename.cor" 2>/dev/null >> mine; ./asm_copy "$filename.s" 2>/dev/null 1>/dev/null ;hexdump "$filename.cor" 2>/dev/null >> true; diff mine true 1>/dev/null 2>/dev/null && echo ">$file SUCCESS" || echo ">$file FAIL"; rm "$filename.cor" true mine 2>/dev/null; done | grep -i FAIL

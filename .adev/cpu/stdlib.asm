:disp
pop r0
:start_disp
mov r1 @r0
cmp 0 r1
jeq end_disp
printc r1
add r0 1
jmp start_disp
:end_disp
return

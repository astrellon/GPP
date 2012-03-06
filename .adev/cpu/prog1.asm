#include "stdlib.asm"

:main
mov r0 5
shftl r0 1
print r0
printc '\n'

shftr r0 1
print r0
printc '\n'

shftr r0 1
print r0
printc '\n'
end

.name "MKP"
.comment "SaR ez Gg"

live:
sti		r1, %:live, %1
lldi	%:live, %:live, r2
lld		%:live, r3
fork	%:live
live	%:live
ld		%:live, r8
and		r3, %:live, r3
or		%:live, %:live, r9
xor		%:live, %:live, r9
zjmp	%:live
ldi		%:live, %:live, r9
sti		r8, %:live, %:live
fork	%:live
lfork	%:live
aff		r4

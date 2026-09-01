	hint_nop49 bx
	hint_nop49 dx
	hint_nop49 edx
	hint_nop49 dword [0xeda]
	hint_nop49 rax
	hint_nop49 rdx

%ifdef ERROR
	hint_nop49 r15w
	hint_nop49 r13d
	hint_nop49 r10
	hint_nop49 r18w
	hint_nop49 r17d
	hint_nop49 r23
%endif

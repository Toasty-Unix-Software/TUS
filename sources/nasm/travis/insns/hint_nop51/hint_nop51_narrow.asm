	hint_nop51 cx
	hint_nop51 si
	hint_nop51 ebp
	hint_nop51 ecx
	hint_nop51 rcx
	hint_nop51 qword [0xf8c]

%ifdef ERROR
	hint_nop51 r10w
	hint_nop51 r12d
	hint_nop51 r12
	hint_nop51 r18w
	hint_nop51 r17d
	hint_nop51 r29
%endif

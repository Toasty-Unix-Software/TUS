	hint_nop14 word [0x68e]
	hint_nop14 bp
	hint_nop14 ecx
	hint_nop14 esi
	hint_nop14 rbx
	hint_nop14 rdi

%ifdef ERROR
	hint_nop14 r8w
	hint_nop14 r13d
	hint_nop14 r10
	hint_nop14 r22w
	hint_nop14 r29d
	hint_nop14 r21
%endif

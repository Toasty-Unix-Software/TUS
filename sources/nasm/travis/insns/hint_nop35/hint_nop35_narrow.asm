	hint_nop35 bx
	hint_nop35 di
	hint_nop35 edi
	hint_nop35 edx
	hint_nop35 rbx
	hint_nop35 rdi

%ifdef ERROR
	hint_nop35 r10w
	hint_nop35 r15d
	hint_nop35 r13
	hint_nop35 r24w
	hint_nop35 r21d
	hint_nop35 r28
%endif

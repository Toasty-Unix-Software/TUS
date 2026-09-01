	hint_nop10 cx
	hint_nop10 ax
	hint_nop10 eax
	hint_nop10 eax
	hint_nop10 rdx
	hint_nop10 rax

%ifdef ERROR
	hint_nop10 r10w
	hint_nop10 r13d
	hint_nop10 r9
	hint_nop10 r23w
	hint_nop10 r24d
	hint_nop10 r30
%endif

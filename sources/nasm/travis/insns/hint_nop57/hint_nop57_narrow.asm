	hint_nop57 word [0x5bb]
	hint_nop57 si
	hint_nop57 esi
	hint_nop57 edi
	hint_nop57 rbp
	hint_nop57 rdx

%ifdef ERROR
	hint_nop57 r13w
	hint_nop57 r9d
	hint_nop57 r10
	hint_nop57 r24w
	hint_nop57 r25d
	hint_nop57 r16
%endif

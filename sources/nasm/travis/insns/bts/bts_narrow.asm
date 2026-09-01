	bts bx, di
	bts bx, si
	bts edi, edi
	bts dword [0xb2b], edi
	bts dx, 0x4a
	bts ax, 0x74

%ifdef ERROR
	bts rcx, rbp
	bts qword [0x1c1], rsi
	bts r12w, r11w
	bts r9d, r12d
	bts r9, r8
	bts r10w, 0x2f
	bts r28w, r21w
	bts r31d, r28d
	bts r17, r24
	bts r18w, 0x19
	bts qword [eax+1], rdx
	bts qword [eax+64], rdi
	bts [0xd38], rsi
%endif

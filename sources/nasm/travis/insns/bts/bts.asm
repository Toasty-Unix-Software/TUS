default rel
	bts bx, di
	bts bx, si
	bts edi, edi
	bts dword [0xb2b], edi
	bts rcx, rbp
	bts qword [0x1c1], rsi
	bts dx, 0x4a
	bts ax, 0x74
	bts r12w, r11w
	bts r9d, r12d
	bts r9, r8
	bts r10w, 0x2f
	bts r28w, r21w
	bts r31d, r28d
	bts r17, r24
	bts r18w, 0x19
	bts word [eax+1], si
	bts word [eax+64], bp
	bts dword [eax+1], eax
	bts dword [eax+64], edx
	bts qword [eax+1], rdx
	bts qword [eax+64], rdi
	bts word [eax+1], 0xb9
	bts word [eax+64], 0x6
	bts dword [eax+1], 0xda
	bts dword [eax+64], 0x2b
	bts qword [eax+1], 0x50
	bts qword [eax+64], 0x8e
	bts [0xda5], di
	bts [0x147], edi
	bts [0xd38], rsi
	bts [0xa5b], 0xfc
	bts [0xdcd], 0xe1
	bts [0xc93], 0x7

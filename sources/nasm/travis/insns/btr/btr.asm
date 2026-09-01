default rel
	btr dx, bx
	btr ax, bx
	btr dword [0xdda], edx
	btr ebx, edi
	btr qword [0x72b], rbx
	btr qword [0xc5a], rbp
	btr si, 0xeb
	btr cx, 0xf6
	btr r10w, r14w
	btr r15d, r9d
	btr r15, r8
	btr r15w, 0xcc
	btr r25w, r30w
	btr r21d, r28d
	btr r22, r26
	btr r24w, 0xad
	btr word [eax+1], cx
	btr word [eax+64], cx
	btr dword [eax+1], edi
	btr dword [eax+64], ecx
	btr qword [eax+1], rbx
	btr qword [eax+64], rcx
	btr word [eax+1], 0xf5
	btr word [eax+64], 0xb5
	btr dword [eax+1], 0xb4
	btr dword [eax+64], 0x8c
	btr qword [eax+1], 0xd8
	btr qword [eax+64], 0xa5
	btr [0xa61], cx
	btr [0x6ee], ebx
	btr [0xc62], rbp
	btr [0x897], 0x65
	btr [0x45c], 0xb4
	btr [0x7a4], 0x95

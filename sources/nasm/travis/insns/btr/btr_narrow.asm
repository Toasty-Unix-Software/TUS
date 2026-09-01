	btr dx, bx
	btr ax, bx
	btr dword [0xdda], edx
	btr ebx, edi
	btr si, 0xeb
	btr cx, 0xf6

%ifdef ERROR
	btr qword [0x72b], rbx
	btr qword [0xc5a], rbp
	btr r10w, r14w
	btr r15d, r9d
	btr r15, r8
	btr r15w, 0xcc
	btr r25w, r30w
	btr r21d, r28d
	btr r22, r26
	btr r24w, 0xad
	btr qword [eax+1], rbx
	btr qword [eax+64], rcx
	btr [0xc62], rbp
%endif

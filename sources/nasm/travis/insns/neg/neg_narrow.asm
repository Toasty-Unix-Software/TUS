	neg byte [0x6aa]
	neg dl
	neg dx
	neg word [0x652]
	neg esi
	neg eax
	neg qword [0x4bf]
	neg rdi

%ifdef ERROR
	neg r12b
	neg r13w
	neg r8d
	neg r10
	neg r31b
	neg r18w
	neg r21d
	neg r20
	neg rdx, qword [eax+1]
	neg rcx, qword [eax+64]
	neg rcx, [0x734]
%endif

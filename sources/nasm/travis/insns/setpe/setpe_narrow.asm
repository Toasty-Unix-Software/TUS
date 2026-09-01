	setpe dl
	setpe byte [0xb26]
	setpe ebp
	setpe esi

%ifdef ERROR
	setpe rbx
	setpe rcx
	setpe r15b
	setpe r15
	setpe r9d
	setpe r31b
	setpe r19
	setpe r31d
%endif

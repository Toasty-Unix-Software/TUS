	pextrq qword [0x204], xmm7, 0xbb
	pextrq qword [0x891], xmm4, 0x61

%ifdef ERROR
	pextrq r14, xmm8, 0xc0
%endif

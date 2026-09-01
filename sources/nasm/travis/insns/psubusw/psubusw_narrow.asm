	psubusw mm7, qword [0x5bd]
	psubusw mm3, qword [0x589]
	psubusw xmm3, oword [0xc9e]
	psubusw xmm2, oword [0x212]

%ifdef ERROR
	psubusw xmm8, xmm9
%endif

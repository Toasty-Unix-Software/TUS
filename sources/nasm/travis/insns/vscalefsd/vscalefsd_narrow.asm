	vscalefsd xmm5, xmm1, qword [0x268]
	vscalefsd xmm5, qword [0x268]
	vscalefsd xmm2, xmm7, xmm2

%ifdef ERROR
	vscalefsd xmm14, xmm12, xmm14
	vscalefsd xmm16, xmm24, xmm25
%endif

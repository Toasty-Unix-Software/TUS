	vscalefss xmm1, xmm3, dword [0xe70]
	vscalefss xmm1, dword [0xe70]
	vscalefss xmm5, xmm4, xmm3

%ifdef ERROR
	vscalefss xmm11, xmm8, xmm9
	vscalefss xmm26, xmm23, xmm29
%endif

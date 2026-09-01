	vcomiss xmm5, xmm7
	vcomiss xmm3, xmm1
	vcomiss xmm5, xmm3
	vcomiss xmm5, dword [0x4a4]

%ifdef ERROR
	vcomiss xmm12, xmm11
	vcomiss xmm8, xmm11
	vcomiss xmm31, xmm26
	vcomiss xmm22, xmm24
%endif

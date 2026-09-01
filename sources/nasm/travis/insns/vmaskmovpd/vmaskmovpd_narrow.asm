	vmaskmovpd xmm3, xmm4, oword [0x3a6]
	vmaskmovpd xmm5, xmm0, oword [0xc2d]
	vmaskmovpd ymm4, ymm7, yword [0x6c6]
	vmaskmovpd ymm7, ymm6, yword [0x68e]
	vmaskmovpd oword [0x240], xmm3, xmm6
	vmaskmovpd oword [0x5e0], xmm5, xmm5
	vmaskmovpd yword [0xa29], ymm6, ymm1
	vmaskmovpd yword [0x489], ymm2, ymm0

%ifdef ERROR
	vmaskmovpd xmm13, xmm15, oword [0xca9]
	vmaskmovpd ymm10, ymm12, yword [0x491]
	vmaskmovpd oword [0x405], xmm12, xmm12
	vmaskmovpd yword [0x877], ymm10, ymm9
%endif

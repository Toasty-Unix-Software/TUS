	vaddsubpd xmm1, xmm2, xmm2
	vaddsubpd xmm1, xmm2
	vaddsubpd xmm1, xmm3, xmm7
	vaddsubpd ymm3, ymm0, yword [0x58b]
	vaddsubpd ymm3, yword [0x58b]
	vaddsubpd ymm1, ymm4, ymm2

%ifdef ERROR
	vaddsubpd xmm8, xmm12, xmm12
	vaddsubpd ymm9, ymm10, ymm12
%endif

	vaddpd xmm7, xmm5, xmm6
	vaddpd xmm7, xmm6
	vaddpd xmm1, xmm0, xmm7
	vaddpd ymm7, ymm2, ymm2
	vaddpd ymm7, ymm2
	vaddpd ymm4, ymm0, yword [0xf04]
	vaddpd xmm6, xmm6, xmm4
	vaddpd xmm6, xmm4
	vaddpd xmm0, xmm4, xmm5
	vaddpd ymm5, ymm3, yword [0xfe1]
	vaddpd ymm5, yword [0xfe1]
	vaddpd ymm5, ymm6, ymm5

%ifdef ERROR
	vaddpd xmm8, xmm8, xmm14
	vaddpd ymm9, ymm14, ymm8
	vaddpd xmm10, xmm9, xmm14
	vaddpd ymm11, ymm11, ymm11
	vaddpd xmm28, xmm25, xmm28
	vaddpd ymm27, ymm30, ymm25
	vaddpd xmm18, xmm22, xmm17
	vaddpd ymm21, ymm31, ymm22
%endif

	vsubpd xmm0, xmm2, oword [0x89d]
	vsubpd xmm0, oword [0x89d]
	vsubpd xmm2, xmm5, xmm3
	vsubpd ymm2, ymm7, ymm3
	vsubpd ymm2, ymm3
	vsubpd ymm3, ymm6, yword [0xa51]
	vsubpd xmm0, xmm3, xmm1
	vsubpd xmm0, xmm1
	vsubpd xmm3, xmm0, xmm0
	vsubpd ymm5, ymm2, yword [0xb81]
	vsubpd ymm5, yword [0xb81]
	vsubpd ymm6, ymm5, ymm0

%ifdef ERROR
	vsubpd xmm10, xmm12, xmm12
	vsubpd ymm12, ymm11, ymm14
	vsubpd xmm12, xmm10, xmm9
	vsubpd ymm8, ymm15, ymm15
	vsubpd xmm24, xmm30, xmm30
	vsubpd ymm16, ymm23, ymm21
	vsubpd xmm23, xmm16, xmm25
	vsubpd ymm31, ymm19, ymm18
%endif

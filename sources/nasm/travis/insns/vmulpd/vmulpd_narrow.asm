	vmulpd xmm3, xmm3, xmm6
	vmulpd xmm3, xmm6
	vmulpd xmm2, xmm3, xmm1
	vmulpd ymm5, ymm7, yword [0x8aa]
	vmulpd ymm5, yword [0x8aa]
	vmulpd ymm1, ymm6, ymm1
	vmulpd xmm5, xmm7, oword [0xf73]
	vmulpd xmm5, oword [0xf73]
	vmulpd xmm7, xmm3, oword [0xc19]
	vmulpd ymm0, ymm0, ymm6
	vmulpd ymm0, ymm6
	vmulpd ymm2, ymm6, ymm5

%ifdef ERROR
	vmulpd xmm10, xmm14, xmm8
	vmulpd ymm10, ymm9, ymm14
	vmulpd xmm14, xmm8, xmm15
	vmulpd ymm14, ymm8, ymm10
	vmulpd xmm25, xmm18, xmm26
	vmulpd ymm23, ymm20, ymm23
	vmulpd xmm21, xmm31, xmm22
	vmulpd ymm21, ymm28, ymm22
%endif

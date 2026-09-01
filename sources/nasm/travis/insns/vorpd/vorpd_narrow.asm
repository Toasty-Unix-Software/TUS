	vorpd xmm5, xmm1, oword [0x973]
	vorpd xmm5, oword [0x973]
	vorpd xmm6, xmm7, xmm2
	vorpd ymm5, ymm0, yword [0x4e0]
	vorpd ymm5, yword [0x4e0]
	vorpd ymm1, ymm4, ymm2
	vorpd xmm5, xmm4, oword [0xd0a]
	vorpd xmm5, oword [0xd0a]
	vorpd xmm6, xmm4, xmm1
	vorpd ymm1, ymm5, ymm3
	vorpd ymm1, ymm3
	vorpd ymm2, ymm7, ymm7

%ifdef ERROR
	vorpd xmm8, xmm10, xmm12
	vorpd ymm11, ymm15, ymm10
	vorpd xmm10, xmm9, xmm10
	vorpd ymm14, ymm8, ymm15
	vorpd xmm23, xmm20, xmm30
	vorpd ymm26, ymm18, ymm25
	vorpd xmm22, xmm26, xmm19
	vorpd ymm26, ymm28, ymm17
%endif

	vpslld xmm5, xmm0, xmm4
	vpslld xmm5, xmm4
	vpslld xmm6, xmm6, oword [0x79e]
	vpslld xmm2, xmm3, 0x18
	vpslld xmm2, 0x18
	vpslld xmm3, xmm2, 0x65
	vpslld ymm3, ymm0, xmm2
	vpslld ymm3, xmm2
	vpslld ymm1, ymm4, oword [0x460]
	vpslld ymm6, ymm2, 0xb1
	vpslld ymm6, 0xb1
	vpslld ymm2, ymm7, 0xc1

%ifdef ERROR
	vpslld xmm9, xmm15, xmm12
	vpslld xmm13, xmm15, 0xa2
	vpslld ymm13, ymm10, xmm8
	vpslld ymm14, ymm12, 0x8a
	vpslld xmm30, xmm23, xmm19
	vpslld xmm21, xmm24, 0x29
	vpslld ymm30, ymm19, xmm31
	vpslld ymm17, ymm21, 0x3e
%endif

	vpsrld xmm6, xmm5, xmm3
	vpsrld xmm6, xmm3
	vpsrld xmm4, xmm5, oword [0xac5]
	vpsrld xmm7, xmm7, 0x28
	vpsrld xmm7, 0x28
	vpsrld xmm5, xmm5, 0x12
	vpsrld ymm6, ymm2, oword [0x485]
	vpsrld ymm6, oword [0x485]
	vpsrld ymm6, ymm2, xmm5
	vpsrld ymm0, ymm7, 0xa2
	vpsrld ymm0, 0xa2
	vpsrld ymm3, ymm6, 0x25

%ifdef ERROR
	vpsrld xmm11, xmm15, xmm15
	vpsrld xmm9, xmm14, 0xb8
	vpsrld ymm13, ymm8, xmm8
	vpsrld ymm13, ymm8, 0x7b
	vpsrld xmm18, xmm19, xmm16
	vpsrld xmm26, xmm28, 0x34
	vpsrld ymm30, ymm25, xmm18
	vpsrld ymm21, ymm25, 0x7a
%endif

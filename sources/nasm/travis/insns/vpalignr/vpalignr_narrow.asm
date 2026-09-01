	vpalignr xmm5, xmm7, xmm5, 0x2d
	vpalignr xmm5, xmm5, 0x2d
	vpalignr xmm7, xmm6, xmm3, 0x26
	vpalignr ymm2, ymm0, ymm3, 0xc4
	vpalignr ymm2, ymm3, 0xc4
	vpalignr ymm5, ymm6, yword [0xe72], 0xf1
	vpalignr xmm3, xmm2, xmm4, 0x41
	vpalignr xmm3, xmm4, 0x41
	vpalignr xmm6, xmm6, oword [0x5c3], 0xd1
	vpalignr ymm4, ymm7, yword [0x95d], 0xf6
	vpalignr ymm4, yword [0x95d], 0xf6
	vpalignr ymm4, ymm7, yword [0xb6c], 0xc6

%ifdef ERROR
	vpalignr xmm13, xmm14, xmm14, 0xad
	vpalignr ymm8, ymm9, ymm12, 0xad
	vpalignr xmm11, xmm14, xmm9, 0x28
	vpalignr ymm10, ymm12, ymm10, 0xc0
	vpalignr xmm30, xmm30, xmm23, 0xd9
	vpalignr ymm30, ymm24, ymm25, 0xbd
	vpalignr xmm27, xmm30, xmm16, 0xd6
	vpalignr ymm27, ymm16, ymm20, 0xab
%endif

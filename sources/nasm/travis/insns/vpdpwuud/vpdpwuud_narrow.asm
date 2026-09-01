	vpdpwuud xmm3, xmm6, xmm3
	vpdpwuud xmm6, xmm4, xmm3
	vpdpwuud ymm0, ymm5, yword [0x66e]
	vpdpwuud ymm0, ymm4, yword [0xb71]
	vpdpwuud xmm5, xmm1, xmm0
	vpdpwuud xmm1, xmm1, xmm4
	vpdpwuud ymm0, ymm6, ymm2
	vpdpwuud ymm5, ymm7, ymm2

%ifdef ERROR
	vpdpwuud xmm13, xmm9, xmm14
	vpdpwuud ymm12, ymm14, ymm9
	vpdpwuud xmm15, xmm14, xmm15
	vpdpwuud ymm8, ymm14, ymm8
	vpdpwuud xmm31, xmm27, xmm26
	vpdpwuud ymm23, ymm19, ymm30
	vpdpwuud xmm24, xmm22, xmm28
	vpdpwuud ymm23, ymm19, ymm19
%endif

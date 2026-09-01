	vpaddd xmm7, xmm3, xmm0
	vpaddd xmm7, xmm0
	vpaddd xmm4, xmm5, xmm6
	vpaddd ymm6, ymm5, yword [0x893]
	vpaddd ymm6, yword [0x893]
	vpaddd ymm4, ymm2, yword [0x41a]
	vpaddd xmm5, xmm3, xmm1
	vpaddd xmm5, xmm1
	vpaddd xmm1, xmm3, xmm4
	vpaddd ymm2, ymm7, ymm7
	vpaddd ymm2, ymm7
	vpaddd ymm2, ymm7, ymm2

%ifdef ERROR
	vpaddd xmm15, xmm14, xmm8
	vpaddd ymm10, ymm9, ymm12
	vpaddd xmm8, xmm13, xmm15
	vpaddd ymm9, ymm9, ymm14
	vpaddd xmm31, xmm23, xmm16
	vpaddd ymm31, ymm30, ymm30
	vpaddd xmm19, xmm31, xmm18
	vpaddd ymm24, ymm30, ymm23
%endif

	vpdpbuud xmm3, xmm3, oword [0xa1c]
	vpdpbuud xmm0, xmm4, xmm7
	vpdpbuud ymm2, ymm3, ymm2
	vpdpbuud ymm7, ymm1, ymm7
	vpdpbuud xmm2, xmm7, xmm3
	vpdpbuud xmm3, xmm6, xmm5
	vpdpbuud ymm4, ymm7, ymm2
	vpdpbuud ymm7, ymm2, yword [0x50b]

%ifdef ERROR
	vpdpbuud xmm14, xmm14, xmm13
	vpdpbuud ymm11, ymm15, ymm14
	vpdpbuud xmm8, xmm8, xmm11
	vpdpbuud ymm8, ymm13, ymm8
	vpdpbuud xmm16, xmm29, xmm28
	vpdpbuud ymm19, ymm31, ymm29
	vpdpbuud xmm31, xmm23, xmm28
	vpdpbuud ymm26, ymm24, ymm23
%endif

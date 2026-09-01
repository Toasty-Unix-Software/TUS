	vaesdec xmm5, xmm7, xmm3
	vaesdec xmm5, xmm3
	vaesdec xmm7, xmm5, xmm6
	vaesdec ymm6, ymm1, ymm7
	vaesdec ymm6, ymm7
	vaesdec ymm4, ymm1, ymm0
	vaesdec zmm4, zmm7, zword [0x5ec]
	vaesdec zmm4, zword [0x5ec]
	vaesdec zmm6, zmm2, zmm0

%ifdef ERROR
	vaesdec xmm13, xmm13, xmm10
	vaesdec ymm14, ymm12, ymm12
	vaesdec zmm10, zmm11, zmm10
	vaesdec xmm17, xmm31, xmm19
	vaesdec ymm21, ymm31, ymm29
	vaesdec zmm19, zmm16, zmm23
%endif

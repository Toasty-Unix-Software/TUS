default rel
	vaesdec xmm5, xmm7, xmm3
	vaesdec xmm5, xmm3
	vaesdec xmm7, xmm5, xmm6
	vaesdec ymm6, ymm1, ymm7
	vaesdec ymm6, ymm7
	vaesdec ymm4, ymm1, ymm0
	vaesdec zmm4, zmm7, zword [0x5ec]
	vaesdec zmm4, zword [0x5ec]
	vaesdec zmm6, zmm2, zmm0
	vaesdec xmm13, xmm13, xmm10
	vaesdec ymm14, ymm12, ymm12
	vaesdec zmm10, zmm11, zmm10
	vaesdec xmm17, xmm31, xmm19
	vaesdec ymm21, ymm31, ymm29
	vaesdec zmm19, zmm16, zmm23
	vaesdec xmm2, xmm0, oword [eax+1]
	vaesdec xmm7, xmm5, oword [eax+64]
	vaesdec ymm5, ymm7, yword [eax+1]
	vaesdec ymm4, ymm7, yword [eax+64]
	vaesdec zmm0, zmm7, zword [eax+1]
	vaesdec zmm2, zmm0, zword [eax+64]
	vaesdec xmm1, xmm7, [0x84d]
	vaesdec ymm0, ymm6, [0x505]
	vaesdec zmm0, zmm3, [0x2e3]

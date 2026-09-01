default rel
	vpermd ymm4, ymm2, ymm3
	vpermd ymm4, ymm3
	vpermd ymm5, ymm3, yword [0x290]
	vpermd ymm3, ymm0, ymm1
	vpermd ymm3, ymm1
	vpermd ymm0, ymm4, ymm5
	vpermd zmm0, zmm5, zmm0
	vpermd zmm0, zmm0
	vpermd zmm3, zmm1, zmm7
	vpermd ymm11, ymm11, ymm12
	vpermd ymm9, ymm10, ymm13
	vpermd zmm12, zmm13, zmm10
	vpermd ymm31, ymm24, ymm21
	vpermd ymm21, ymm21, ymm17
	vpermd zmm19, zmm19, zmm29
	vpermd ymm4{k3}, ymm3, ymm2
	vpermd zmm2{k4}, zmm3, zword [0x859]
	vpermd ymm6{k4}{z}, ymm2, yword [0xa08]
	vpermd zmm4{k1}{z}, zmm0, zmm0
	vpermd ymm6, ymm2, yword [eax+1]
	vpermd ymm5, ymm7, yword [eax+64]
	vpermd ymm2, ymm0, yword [eax+1]
	vpermd ymm3, ymm1, yword [eax+64]
	vpermd zmm7, zmm5, zword [eax+1]
	vpermd zmm7, zmm2, zword [eax+64]
	vpermd ymm5, ymm3, [0x3e2]
	vpermd ymm5, ymm1, [0xc0e]
	vpermd zmm7, zmm6, [0xe81]

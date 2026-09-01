default rel
	vpaddsb xmm7, xmm0, xmm6
	vpaddsb xmm7, xmm6
	vpaddsb xmm3, xmm2, oword [0xf4c]
	vpaddsb ymm0, ymm7, ymm7
	vpaddsb ymm0, ymm7
	vpaddsb ymm0, ymm4, ymm6
	vpaddsb xmm3, xmm6, oword [0xa82]
	vpaddsb xmm3, oword [0xa82]
	vpaddsb xmm6, xmm6, xmm5
	vpaddsb ymm6, ymm2, yword [0xc53]
	vpaddsb ymm6, yword [0xc53]
	vpaddsb ymm0, ymm2, ymm2
	vpaddsb xmm11, xmm14, xmm12
	vpaddsb ymm12, ymm11, ymm12
	vpaddsb xmm12, xmm11, xmm12
	vpaddsb ymm9, ymm9, ymm15
	vpaddsb xmm31, xmm21, xmm20
	vpaddsb ymm29, ymm29, ymm25
	vpaddsb xmm20, xmm16, xmm18
	vpaddsb ymm31, ymm29, ymm28
	vpaddsb xmm5{k4}, xmm6, xmm5
	vpaddsb ymm7{k6}, ymm0, ymm4
	vpaddsb zmm4{k7}, zmm7, zword [0x717]
	vpaddsb xmm2{k1}{z}, xmm4, xmm3
	vpaddsb ymm1{k3}{z}, ymm6, yword [0xa67]
	vpaddsb zmm0{k7}{z}, zmm2, zword [0xe01]
	vpaddsb xmm7, xmm7, oword [eax+1]
	vpaddsb xmm2, xmm6, oword [eax+64]
	vpaddsb ymm7, ymm4, yword [eax+1]
	vpaddsb ymm4, ymm0, yword [eax+64]
	vpaddsb xmm1, xmm6, oword [eax+1]
	vpaddsb xmm2, xmm6, oword [eax+64]
	vpaddsb ymm3, ymm6, yword [eax+1]
	vpaddsb ymm1, ymm2, yword [eax+64]
	vpaddsb zmm4, zmm3, zword [eax+1]
	vpaddsb zmm3, zmm7, zword [eax+64]
	vpaddsb xmm1, xmm1, [0x44b]
	vpaddsb ymm7, ymm6, [0x3ec]
	vpaddsb xmm5, xmm3, [0x77f]
	vpaddsb ymm6, ymm5, [0xba4]
	vpaddsb zmm1, zmm6, [0x83a]

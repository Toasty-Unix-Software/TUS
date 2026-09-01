default rel
	vpermi2ps xmm3, xmm7, xmm4
	vpermi2ps xmm7, xmm7, oword [0xb1b]
	vpermi2ps ymm7, ymm2, ymm1
	vpermi2ps ymm3, ymm2, ymm7
	vpermi2ps zmm4, zmm6, zmm4
	vpermi2ps zmm2, zmm2, zmm0
	vpermi2ps xmm12, xmm10, xmm10
	vpermi2ps ymm11, ymm10, ymm15
	vpermi2ps zmm14, zmm13, zmm11
	vpermi2ps xmm24, xmm22, xmm18
	vpermi2ps ymm24, ymm20, ymm28
	vpermi2ps zmm29, zmm29, zmm28
	vpermi2ps xmm2{k4}, xmm4, oword [0xdb6]
	vpermi2ps ymm6{k2}, ymm3, ymm3
	vpermi2ps zmm2{k4}, zmm4, zmm4
	vpermi2ps xmm7{k3}{z}, xmm6, xmm0
	vpermi2ps ymm2{k3}{z}, ymm2, ymm0
	vpermi2ps zmm4{k6}{z}, zmm6, zword [0xded]
	vpermi2ps xmm3, xmm2, oword [eax+1]
	vpermi2ps xmm1, xmm7, oword [eax+64]
	vpermi2ps ymm0, ymm6, yword [eax+1]
	vpermi2ps ymm4, ymm5, yword [eax+64]
	vpermi2ps zmm1, zmm2, zword [eax+1]
	vpermi2ps zmm7, zmm4, zword [eax+64]
	vpermi2ps xmm2, xmm2, [0x49e]
	vpermi2ps ymm4, ymm5, [0x9e9]
	vpermi2ps zmm0, zmm3, [0x8f7]

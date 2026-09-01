default rel
	vpermi2d xmm4, xmm2, xmm5
	vpermi2d xmm1, xmm1, xmm4
	vpermi2d ymm6, ymm5, yword [0xb13]
	vpermi2d ymm5, ymm0, ymm4
	vpermi2d zmm1, zmm6, zmm2
	vpermi2d zmm4, zmm3, zmm2
	vpermi2d xmm10, xmm8, xmm13
	vpermi2d ymm10, ymm11, ymm10
	vpermi2d zmm10, zmm10, zmm8
	vpermi2d xmm30, xmm23, xmm28
	vpermi2d ymm16, ymm29, ymm26
	vpermi2d zmm20, zmm16, zmm16
	vpermi2d xmm0{k2}, xmm0, oword [0xd9b]
	vpermi2d ymm2{k1}, ymm1, ymm4
	vpermi2d zmm3{k6}, zmm3, zmm3
	vpermi2d xmm4{k3}{z}, xmm6, oword [0xf82]
	vpermi2d ymm1{k3}{z}, ymm1, yword [0x5ca]
	vpermi2d zmm3{k3}{z}, zmm5, zmm1
	vpermi2d xmm0, xmm6, oword [eax+1]
	vpermi2d xmm3, xmm1, oword [eax+64]
	vpermi2d ymm3, ymm3, yword [eax+1]
	vpermi2d ymm4, ymm5, yword [eax+64]
	vpermi2d zmm1, zmm3, zword [eax+1]
	vpermi2d zmm6, zmm2, zword [eax+64]
	vpermi2d xmm0, xmm6, [0xc54]
	vpermi2d ymm0, ymm0, [0xdf9]
	vpermi2d zmm7, zmm4, [0xd2a]

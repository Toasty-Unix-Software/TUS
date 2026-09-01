default rel
	vpermt2w xmm7, xmm6, xmm4
	vpermt2w xmm3, xmm6, xmm0
	vpermt2w ymm7, ymm4, yword [0x947]
	vpermt2w ymm4, ymm7, ymm3
	vpermt2w zmm6, zmm6, zmm5
	vpermt2w zmm4, zmm0, zmm2
	vpermt2w xmm14, xmm11, xmm14
	vpermt2w ymm14, ymm8, ymm13
	vpermt2w zmm14, zmm12, zmm8
	vpermt2w xmm19, xmm17, xmm26
	vpermt2w ymm29, ymm31, ymm16
	vpermt2w zmm28, zmm19, zmm22
	vpermt2w xmm2{k1}, xmm5, oword [0x7e1]
	vpermt2w ymm3{k2}, ymm0, ymm7
	vpermt2w zmm6{k6}, zmm2, zmm3
	vpermt2w xmm7{k2}{z}, xmm7, xmm3
	vpermt2w ymm3{k5}{z}, ymm0, ymm6
	vpermt2w zmm1{k2}{z}, zmm1, zmm4
	vpermt2w xmm2, xmm0, oword [eax+1]
	vpermt2w xmm6, xmm2, oword [eax+64]
	vpermt2w ymm1, ymm4, yword [eax+1]
	vpermt2w ymm0, ymm3, yword [eax+64]
	vpermt2w zmm6, zmm7, zword [eax+1]
	vpermt2w zmm1, zmm0, zword [eax+64]
	vpermt2w xmm2, xmm4, [0x3a4]
	vpermt2w ymm2, ymm2, [0x7b5]
	vpermt2w zmm7, zmm5, [0xab7]

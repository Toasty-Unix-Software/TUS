default rel
	vprord xmm6, xmm7, 0x46
	vprord xmm6, 0x46
	vprord xmm7, xmm2, 0x79
	vprord ymm6, ymm1, 0x6e
	vprord ymm6, 0x6e
	vprord ymm5, ymm4, 0xd3
	vprord zmm0, zmm0, 0x7d
	vprord zmm0, 0x7d
	vprord zmm1, zword [0x247], 0x2
	vprord xmm11, xmm14, 0x18
	vprord ymm10, ymm8, 0x7a
	vprord zmm9, zmm15, 0x70
	vprord xmm16, xmm19, 0x24
	vprord ymm17, ymm20, 0xea
	vprord zmm23, zmm31, 0x89
	vprord xmm5{k4}, oword [0x371], 0xc0
	vprord ymm3{k1}, ymm6, 0x55
	vprord zmm1{k6}, zmm6, 0xad
	vprord xmm7{k7}{z}, xmm3, 0x30
	vprord ymm6{k7}{z}, yword [0x172], 0xd0
	vprord zmm4{k2}{z}, zword [0x517], 0xa2
	vprord xmm1, oword [eax+1], 0x29
	vprord xmm5, oword [eax+64], 0xb1
	vprord ymm5, yword [eax+1], 0x7a
	vprord ymm1, yword [eax+64], 0x83
	vprord zmm2, zword [eax+1], 0x85
	vprord zmm5, zword [eax+64], 0x67
	vprord xmm3, [0x31e], 0x1c
	vprord ymm4, [0xb54], 0x24
	vprord zmm0, [0xf24], 0x79

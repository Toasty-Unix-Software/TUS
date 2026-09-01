default rel
	vxorpd xmm6, xmm6, xmm2
	vxorpd xmm6, xmm2
	vxorpd xmm2, xmm4, xmm2
	vxorpd ymm0, ymm6, ymm3
	vxorpd ymm0, ymm3
	vxorpd ymm2, ymm7, ymm3
	vxorpd xmm2, xmm3, xmm2
	vxorpd xmm2, xmm2
	vxorpd xmm5, xmm3, xmm7
	vxorpd ymm0, ymm4, ymm0
	vxorpd ymm0, ymm0
	vxorpd ymm5, ymm4, ymm2
	vxorpd xmm10, xmm13, xmm15
	vxorpd ymm12, ymm13, ymm8
	vxorpd xmm10, xmm10, xmm15
	vxorpd ymm15, ymm14, ymm15
	vxorpd xmm17, xmm30, xmm30
	vxorpd ymm17, ymm21, ymm31
	vxorpd xmm24, xmm23, xmm16
	vxorpd ymm26, ymm24, ymm17
	vxorpd xmm4{k2}, xmm1, oword [0x4a1]
	vxorpd ymm2{k3}, ymm5, yword [0x5c6]
	vxorpd zmm7{k6}, zmm1, zword [0x180]
	vxorpd xmm2{k6}{z}, xmm6, xmm7
	vxorpd ymm5{k7}{z}, ymm0, yword [0xefa]
	vxorpd zmm2{k4}{z}, zmm5, zword [0x2d2]
	vxorpd xmm4, xmm2, oword [eax+1]
	vxorpd xmm5, xmm2, oword [eax+64]
	vxorpd ymm1, ymm2, yword [eax+1]
	vxorpd ymm4, ymm5, yword [eax+64]
	vxorpd xmm0, xmm2, oword [eax+1]
	vxorpd xmm1, xmm4, oword [eax+64]
	vxorpd ymm5, ymm1, yword [eax+1]
	vxorpd ymm0, ymm1, yword [eax+64]
	vxorpd zmm1, zmm2, zword [eax+1]
	vxorpd zmm2, zmm5, zword [eax+64]
	vxorpd xmm5, xmm5, [0xc35]
	vxorpd ymm6, ymm5, [0x26f]
	vxorpd xmm4, xmm6, [0x25f]
	vxorpd ymm7, ymm0, [0x580]
	vxorpd zmm6, zmm4, [0x95b]

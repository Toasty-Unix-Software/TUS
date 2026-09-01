default rel
	vdivpd xmm5, xmm4, oword [0xbec]
	vdivpd xmm5, oword [0xbec]
	vdivpd xmm5, xmm0, oword [0x9b3]
	vdivpd ymm1, ymm7, ymm2
	vdivpd ymm1, ymm2
	vdivpd ymm1, ymm0, ymm7
	vdivpd xmm0, xmm1, oword [0xd15]
	vdivpd xmm0, oword [0xd15]
	vdivpd xmm5, xmm0, oword [0x86c]
	vdivpd ymm4, ymm0, yword [0x3b3]
	vdivpd ymm4, yword [0x3b3]
	vdivpd ymm5, ymm3, ymm6
	vdivpd xmm14, xmm11, xmm8
	vdivpd ymm8, ymm14, ymm11
	vdivpd xmm9, xmm9, xmm14
	vdivpd ymm14, ymm8, ymm14
	vdivpd xmm30, xmm31, xmm20
	vdivpd ymm30, ymm22, ymm29
	vdivpd xmm23, xmm20, xmm22
	vdivpd ymm20, ymm19, ymm21
	vdivpd xmm3{k6}, xmm2, xmm1
	vdivpd ymm7{k1}, ymm5, ymm0
	vdivpd zmm5{k7}, zmm5, zmm0
	vdivpd xmm4{k1}{z}, xmm5, xmm4
	vdivpd ymm4{k7}{z}, ymm4, ymm1
	vdivpd zmm5{k5}{z}, zmm7, zmm7
	vdivpd zmm3, zmm6, zmm2, {rd-sae}
	vdivpd xmm4, xmm0, oword [eax+1]
	vdivpd xmm7, xmm6, oword [eax+64]
	vdivpd ymm1, ymm3, yword [eax+1]
	vdivpd ymm1, ymm7, yword [eax+64]
	vdivpd xmm3, xmm1, oword [eax+1]
	vdivpd xmm3, xmm2, oword [eax+64]
	vdivpd ymm4, ymm1, yword [eax+1]
	vdivpd ymm6, ymm6, yword [eax+64]
	vdivpd zmm3, zmm3, zword [eax+1]
	vdivpd zmm6, zmm2, zword [eax+64]
	vdivpd xmm5, xmm0, [0x37d]
	vdivpd ymm4, ymm6, [0x79b]
	vdivpd xmm7, xmm3, [0xec2]
	vdivpd ymm1, ymm3, [0xa9a]
	vdivpd zmm6, zmm1, [0xa08]

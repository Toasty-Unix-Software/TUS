default rel
	vpsrlvd xmm6, xmm6, xmm2
	vpsrlvd xmm6, xmm2
	vpsrlvd xmm4, xmm3, xmm0
	vpsrlvd ymm1, ymm6, ymm5
	vpsrlvd ymm1, ymm5
	vpsrlvd ymm7, ymm6, yword [0x742]
	vpsrlvd xmm6, xmm6, xmm5
	vpsrlvd xmm6, xmm5
	vpsrlvd xmm3, xmm5, xmm1
	vpsrlvd ymm3, ymm1, yword [0xafe]
	vpsrlvd ymm3, yword [0xafe]
	vpsrlvd ymm4, ymm4, ymm4
	vpsrlvd xmm14, xmm15, xmm14
	vpsrlvd ymm11, ymm9, ymm11
	vpsrlvd xmm10, xmm11, xmm8
	vpsrlvd ymm9, ymm13, ymm13
	vpsrlvd xmm21, xmm26, xmm19
	vpsrlvd ymm17, ymm31, ymm25
	vpsrlvd xmm22, xmm20, xmm21
	vpsrlvd ymm19, ymm23, ymm25
	vpsrlvd xmm7{k2}, xmm1, xmm5
	vpsrlvd ymm3{k7}, ymm0, yword [0xcf1]
	vpsrlvd zmm6{k3}, zmm4, zmm6
	vpsrlvd xmm4{k7}{z}, xmm2, xmm7
	vpsrlvd ymm1{k4}{z}, ymm7, ymm4
	vpsrlvd zmm4{k2}{z}, zmm2, zmm2
	vpsrlvd xmm7, xmm7, oword [eax+1]
	vpsrlvd xmm6, xmm0, oword [eax+64]
	vpsrlvd ymm6, ymm6, yword [eax+1]
	vpsrlvd ymm0, ymm0, yword [eax+64]
	vpsrlvd xmm6, xmm6, oword [eax+1]
	vpsrlvd xmm4, xmm2, oword [eax+64]
	vpsrlvd ymm5, ymm0, yword [eax+1]
	vpsrlvd ymm3, ymm0, yword [eax+64]
	vpsrlvd zmm0, zmm2, zword [eax+1]
	vpsrlvd zmm0, zmm2, zword [eax+64]
	vpsrlvd xmm2, xmm4, [0x116]
	vpsrlvd ymm4, ymm7, [0xfea]
	vpsrlvd xmm7, xmm5, [0x9f1]
	vpsrlvd ymm4, ymm4, [0xd09]
	vpsrlvd zmm4, zmm7, [0x252]

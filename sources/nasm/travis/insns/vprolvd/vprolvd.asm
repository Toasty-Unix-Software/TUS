default rel
	vprolvd xmm7, xmm5, xmm3
	vprolvd xmm7, xmm3
	vprolvd xmm6, xmm0, xmm3
	vprolvd ymm7, ymm4, ymm4
	vprolvd ymm7, ymm4
	vprolvd ymm4, ymm5, ymm3
	vprolvd zmm0, zmm3, zmm5
	vprolvd zmm0, zmm5
	vprolvd zmm5, zmm0, zmm2
	vprolvd xmm10, xmm12, xmm10
	vprolvd ymm10, ymm10, ymm10
	vprolvd zmm9, zmm13, zmm10
	vprolvd xmm30, xmm31, xmm24
	vprolvd ymm26, ymm28, ymm28
	vprolvd zmm22, zmm22, zmm23
	vprolvd xmm7{k6}, xmm5, xmm0
	vprolvd ymm7{k2}, ymm4, yword [0x741]
	vprolvd zmm6{k4}, zmm7, zword [0x380]
	vprolvd xmm3{k5}{z}, xmm3, oword [0xbd9]
	vprolvd ymm4{k6}{z}, ymm3, ymm4
	vprolvd zmm2{k3}{z}, zmm4, zmm0
	vprolvd xmm0, xmm6, oword [eax+1]
	vprolvd xmm1, xmm4, oword [eax+64]
	vprolvd ymm4, ymm0, yword [eax+1]
	vprolvd ymm5, ymm7, yword [eax+64]
	vprolvd zmm0, zmm5, zword [eax+1]
	vprolvd zmm4, zmm2, zword [eax+64]
	vprolvd xmm0, xmm6, [0x2d7]
	vprolvd ymm0, ymm0, [0x9ed]
	vprolvd zmm1, zmm3, [0x7d8]

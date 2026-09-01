default rel
	vprolvq xmm6, xmm5, oword [0x550]
	vprolvq xmm6, oword [0x550]
	vprolvq xmm7, xmm2, oword [0x219]
	vprolvq ymm0, ymm2, yword [0x3a7]
	vprolvq ymm0, yword [0x3a7]
	vprolvq ymm4, ymm5, yword [0xe4b]
	vprolvq zmm7, zmm4, zmm7
	vprolvq zmm7, zmm7
	vprolvq zmm6, zmm6, zword [0xb76]
	vprolvq xmm13, xmm15, xmm13
	vprolvq ymm9, ymm9, ymm11
	vprolvq zmm14, zmm10, zmm14
	vprolvq xmm31, xmm26, xmm22
	vprolvq ymm28, ymm30, ymm22
	vprolvq zmm23, zmm18, zmm24
	vprolvq xmm3{k2}, xmm6, xmm5
	vprolvq ymm6{k6}, ymm4, ymm6
	vprolvq zmm4{k4}, zmm7, zmm3
	vprolvq xmm4{k1}{z}, xmm3, xmm5
	vprolvq ymm2{k3}{z}, ymm7, yword [0x927]
	vprolvq zmm2{k5}{z}, zmm7, zmm2
	vprolvq xmm3, xmm7, oword [eax+1]
	vprolvq xmm6, xmm5, oword [eax+64]
	vprolvq ymm0, ymm3, yword [eax+1]
	vprolvq ymm2, ymm3, yword [eax+64]
	vprolvq zmm7, zmm1, zword [eax+1]
	vprolvq zmm4, zmm7, zword [eax+64]
	vprolvq xmm4, xmm2, [0x6ea]
	vprolvq ymm2, ymm0, [0x1fd]
	vprolvq zmm5, zmm0, [0xeec]

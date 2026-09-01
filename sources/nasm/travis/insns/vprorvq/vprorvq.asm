default rel
	vprorvq xmm6, xmm0, xmm4
	vprorvq xmm6, xmm4
	vprorvq xmm0, xmm6, oword [0x547]
	vprorvq ymm1, ymm6, yword [0x688]
	vprorvq ymm1, yword [0x688]
	vprorvq ymm0, ymm5, ymm6
	vprorvq zmm0, zmm6, zmm2
	vprorvq zmm0, zmm2
	vprorvq zmm4, zmm6, zmm2
	vprorvq xmm11, xmm15, xmm14
	vprorvq ymm14, ymm10, ymm12
	vprorvq zmm8, zmm12, zmm8
	vprorvq xmm16, xmm20, xmm22
	vprorvq ymm28, ymm22, ymm28
	vprorvq zmm20, zmm23, zmm22
	vprorvq xmm0{k6}, xmm7, xmm1
	vprorvq ymm2{k6}, ymm7, ymm4
	vprorvq zmm7{k7}, zmm3, zmm7
	vprorvq xmm1{k7}{z}, xmm5, xmm5
	vprorvq ymm7{k7}{z}, ymm7, ymm1
	vprorvq zmm2{k6}{z}, zmm1, zword [0x3d9]
	vprorvq xmm2, xmm4, oword [eax+1]
	vprorvq xmm1, xmm3, oword [eax+64]
	vprorvq ymm4, ymm2, yword [eax+1]
	vprorvq ymm5, ymm4, yword [eax+64]
	vprorvq zmm6, zmm6, zword [eax+1]
	vprorvq zmm2, zmm2, zword [eax+64]
	vprorvq xmm6, xmm6, [0x6cc]
	vprorvq ymm4, ymm0, [0xd78]
	vprorvq zmm1, zmm4, [0xd38]

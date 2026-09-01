default rel
	vpsllvq xmm7, xmm0, oword [0x74d]
	vpsllvq xmm7, oword [0x74d]
	vpsllvq xmm7, xmm2, xmm1
	vpsllvq ymm1, ymm2, ymm2
	vpsllvq ymm1, ymm2
	vpsllvq ymm7, ymm0, ymm5
	vpsllvq xmm7, xmm0, xmm4
	vpsllvq xmm7, xmm4
	vpsllvq xmm0, xmm7, xmm0
	vpsllvq ymm3, ymm1, yword [0x86d]
	vpsllvq ymm3, yword [0x86d]
	vpsllvq ymm2, ymm1, ymm3
	vpsllvq xmm10, xmm13, xmm13
	vpsllvq ymm12, ymm11, ymm11
	vpsllvq xmm14, xmm8, xmm14
	vpsllvq ymm10, ymm13, ymm15
	vpsllvq xmm22, xmm31, xmm21
	vpsllvq ymm23, ymm20, ymm28
	vpsllvq xmm27, xmm22, xmm30
	vpsllvq ymm22, ymm28, ymm18
	vpsllvq xmm5{k6}, xmm0, xmm4
	vpsllvq ymm0{k7}, ymm6, ymm7
	vpsllvq zmm2{k5}, zmm6, zmm2
	vpsllvq xmm2{k7}{z}, xmm4, xmm7
	vpsllvq ymm6{k4}{z}, ymm7, ymm0
	vpsllvq zmm1{k3}{z}, zmm7, zmm1
	vpsllvq xmm3, xmm0, oword [eax+1]
	vpsllvq xmm0, xmm5, oword [eax+64]
	vpsllvq ymm6, ymm6, yword [eax+1]
	vpsllvq ymm5, ymm0, yword [eax+64]
	vpsllvq xmm4, xmm2, oword [eax+1]
	vpsllvq xmm4, xmm7, oword [eax+64]
	vpsllvq ymm0, ymm7, yword [eax+1]
	vpsllvq ymm1, ymm4, yword [eax+64]
	vpsllvq zmm3, zmm4, zword [eax+1]
	vpsllvq zmm1, zmm3, zword [eax+64]
	vpsllvq xmm1, xmm0, [0xed6]
	vpsllvq ymm1, ymm3, [0x892]
	vpsllvq xmm0, xmm7, [0x10b]
	vpsllvq ymm1, ymm1, [0xc06]
	vpsllvq zmm6, zmm7, [0x20f]

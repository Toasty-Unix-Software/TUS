default rel
	vpsrlq xmm7, xmm5, xmm4
	vpsrlq xmm7, xmm4
	vpsrlq xmm0, xmm5, xmm6
	vpsrlq xmm5, xmm6, 0xb7
	vpsrlq xmm5, 0xb7
	vpsrlq xmm1, xmm1, 0xce
	vpsrlq ymm5, ymm1, xmm3
	vpsrlq ymm5, xmm3
	vpsrlq ymm3, ymm1, xmm4
	vpsrlq ymm2, ymm6, 0x16
	vpsrlq ymm2, 0x16
	vpsrlq ymm1, ymm7, 0xf
	vpsrlq xmm13, xmm14, xmm14
	vpsrlq xmm10, xmm14, 0x88
	vpsrlq ymm11, ymm12, xmm15
	vpsrlq ymm8, ymm11, 0x50
	vpsrlq xmm22, xmm21, xmm19
	vpsrlq xmm24, xmm17, 0x95
	vpsrlq ymm30, ymm18, xmm24
	vpsrlq ymm19, ymm18, 0x11
	vpsrlq xmm7{k1}, xmm5, xmm1
	vpsrlq ymm0{k5}, ymm2, oword [0xe37]
	vpsrlq zmm0{k4}, zmm3, oword [0xa2c]
	vpsrlq xmm3{k4}, xmm5, 0xdc
	vpsrlq ymm2{k7}, ymm5, 0xa8
	vpsrlq zmm4{k4}, zmm1, 0xad
	vpsrlq xmm3{k6}{z}, xmm5, oword [0xeb3]
	vpsrlq ymm7{k1}{z}, ymm0, xmm7
	vpsrlq zmm3{k2}{z}, zmm4, xmm7
	vpsrlq xmm5{k4}{z}, xmm3, 0xfe
	vpsrlq ymm5{k3}{z}, ymm3, 0x8c
	vpsrlq zmm3{k5}{z}, zmm7, 0x51
	vpsrlq xmm4, xmm3, oword [eax+1]
	vpsrlq xmm3, xmm5, oword [eax+64]
	vpsrlq ymm3, ymm7, oword [eax+1]
	vpsrlq ymm7, ymm2, oword [eax+64]
	vpsrlq xmm7, xmm1, oword [eax+1]
	vpsrlq xmm2, xmm7, oword [eax+64]
	vpsrlq ymm2, ymm1, oword [eax+1]
	vpsrlq ymm5, ymm5, oword [eax+64]
	vpsrlq zmm4, zmm4, oword [eax+1]
	vpsrlq zmm5, zmm2, oword [eax+64]
	vpsrlq xmm5, oword [eax+1], 0x7c
	vpsrlq xmm6, oword [eax+64], 0xd9
	vpsrlq ymm2, yword [eax+1], 0x1b
	vpsrlq ymm0, yword [eax+64], 0xbb
	vpsrlq zmm0, zword [eax+1], 0x96
	vpsrlq zmm7, zword [eax+64], 0x17
	vpsrlq xmm7, xmm1, [0x926]
	vpsrlq ymm4, ymm0, [0xf27]
	vpsrlq xmm4, xmm5, [0x49c]
	vpsrlq ymm3, ymm7, [0x5e8]
	vpsrlq zmm7, zmm2, [0xb2b]
	vpsrlq xmm7, [0xd5f], 0x4c
	vpsrlq ymm0, [0x439], 0x72
	vpsrlq zmm4, [0xaf7], 0xfc

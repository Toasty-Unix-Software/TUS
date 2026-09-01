default rel
	vprorq xmm7, oword [0x249], 0x45
	vprorq xmm7, 0x45
	vprorq xmm0, oword [0xcc2], 0xcc
	vprorq ymm5, ymm2, 0x3b
	vprorq ymm5, 0x3b
	vprorq ymm6, ymm0, 0x38
	vprorq zmm5, zmm6, 0xe4
	vprorq zmm5, 0xe4
	vprorq zmm0, zword [0xd60], 0xe0
	vprorq xmm13, xmm9, 0xa7
	vprorq ymm10, ymm12, 0x9c
	vprorq zmm14, zmm14, 0x92
	vprorq xmm21, xmm29, 0xe7
	vprorq ymm31, ymm31, 0xd6
	vprorq zmm24, zmm29, 0xe5
	vprorq xmm4{k2}, xmm4, 0xe9
	vprorq ymm1{k6}, ymm3, 0x4c
	vprorq zmm4{k2}, zword [0x439], 0xba
	vprorq xmm1{k1}{z}, oword [0xe46], 0x16
	vprorq ymm0{k4}{z}, ymm4, 0xca
	vprorq zmm5{k4}{z}, zword [0x286], 0x8e
	vprorq xmm4, oword [eax+1], 0x38
	vprorq xmm7, oword [eax+64], 0x66
	vprorq ymm5, yword [eax+1], 0x39
	vprorq ymm7, yword [eax+64], 0x4a
	vprorq zmm5, zword [eax+1], 0xc1
	vprorq zmm0, zword [eax+64], 0x2e
	vprorq xmm6, [0x317], 0xc7
	vprorq ymm6, [0xabc], 0x40
	vprorq zmm6, [0x34f], 0x54

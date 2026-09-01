default rel
	vpshldvq xmm3, xmm6, xmm7
	vpshldvq xmm3, xmm7
	vpshldvq xmm7, xmm1, oword [0xbe7]
	vpshldvq ymm0, ymm6, yword [0xb27]
	vpshldvq ymm0, yword [0xb27]
	vpshldvq ymm4, ymm4, ymm6
	vpshldvq zmm0, zmm2, zmm6
	vpshldvq zmm0, zmm6
	vpshldvq zmm1, zmm3, zmm3
	vpshldvq xmm11, xmm12, xmm10
	vpshldvq ymm9, ymm12, ymm15
	vpshldvq zmm12, zmm13, zmm13
	vpshldvq xmm28, xmm30, xmm26
	vpshldvq ymm19, ymm16, ymm22
	vpshldvq zmm18, zmm29, zmm27
	vpshldvq xmm4{k5}, xmm7, oword [0xad5]
	vpshldvq ymm2{k4}, ymm0, ymm1
	vpshldvq zmm6{k1}, zmm2, zword [0xedf]
	vpshldvq xmm0{k5}{z}, xmm2, xmm0
	vpshldvq ymm1{k3}{z}, ymm4, ymm3
	vpshldvq zmm1{k4}{z}, zmm5, zmm2
	vpshldvq xmm0, xmm6, oword [eax+1]
	vpshldvq xmm4, xmm6, oword [eax+64]
	vpshldvq ymm5, ymm4, yword [eax+1]
	vpshldvq ymm6, ymm1, yword [eax+64]
	vpshldvq zmm5, zmm4, zword [eax+1]
	vpshldvq zmm3, zmm6, zword [eax+64]
	vpshldvq xmm2, xmm0, [0xa53]
	vpshldvq ymm4, ymm6, [0x13f]
	vpshldvq zmm0, zmm5, [0xca1]

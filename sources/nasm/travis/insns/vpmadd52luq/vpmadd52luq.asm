default rel
	vpmadd52luq xmm4, xmm0, xmm2
	vpmadd52luq xmm2, xmm7, xmm1
	vpmadd52luq ymm7, ymm2, ymm5
	vpmadd52luq ymm4, ymm2, ymm6
	vpmadd52luq xmm0, xmm5, xmm4
	vpmadd52luq xmm3, xmm0, xmm7
	vpmadd52luq ymm5, ymm4, ymm0
	vpmadd52luq ymm4, ymm5, ymm1
	vpmadd52luq xmm8, xmm10, xmm12
	vpmadd52luq ymm14, ymm15, ymm15
	vpmadd52luq xmm11, xmm12, xmm10
	vpmadd52luq ymm9, ymm14, ymm12
	vpmadd52luq xmm16, xmm22, xmm30
	vpmadd52luq ymm21, ymm23, ymm20
	vpmadd52luq xmm30, xmm30, xmm29
	vpmadd52luq ymm24, ymm29, ymm22
	vpmadd52luq xmm0{k6}, xmm0, xmm7
	vpmadd52luq ymm6{k3}, ymm3, yword [0x1b3]
	vpmadd52luq zmm3{k1}, zmm3, zmm7
	vpmadd52luq xmm2{k5}{z}, xmm0, xmm5
	vpmadd52luq ymm2{k4}{z}, ymm5, yword [0xd79]
	vpmadd52luq zmm2{k2}{z}, zmm2, zmm5
	vpmadd52luq xmm7, xmm4, oword [eax+1]
	vpmadd52luq xmm7, xmm5, oword [eax+64]
	vpmadd52luq ymm4, ymm3, yword [eax+1]
	vpmadd52luq ymm6, ymm1, yword [eax+64]
	vpmadd52luq xmm0, xmm3, oword [eax+1]
	vpmadd52luq xmm1, xmm5, oword [eax+64]
	vpmadd52luq ymm4, ymm3, yword [eax+1]
	vpmadd52luq ymm2, ymm1, yword [eax+64]
	vpmadd52luq zmm4, zmm5, zword [eax+1]
	vpmadd52luq zmm4, zmm4, zword [eax+64]
	vpmadd52luq xmm6, xmm6, [0x56d]
	vpmadd52luq ymm4, ymm0, [0xd9e]
	vpmadd52luq xmm4, xmm7, [0xde2]
	vpmadd52luq ymm1, ymm4, [0x872]
	vpmadd52luq zmm4, zmm3, [0x3c4]

default rel
	vpsrlvq xmm4, xmm2, xmm2
	vpsrlvq xmm4, xmm2
	vpsrlvq xmm5, xmm3, xmm1
	vpsrlvq ymm1, ymm4, ymm6
	vpsrlvq ymm1, ymm6
	vpsrlvq ymm4, ymm0, ymm2
	vpsrlvq xmm4, xmm4, xmm0
	vpsrlvq xmm4, xmm0
	vpsrlvq xmm4, xmm4, oword [0x9b8]
	vpsrlvq ymm0, ymm3, ymm1
	vpsrlvq ymm0, ymm1
	vpsrlvq ymm0, ymm5, ymm2
	vpsrlvq xmm10, xmm9, xmm14
	vpsrlvq ymm11, ymm10, ymm9
	vpsrlvq xmm14, xmm15, xmm15
	vpsrlvq ymm10, ymm15, ymm10
	vpsrlvq xmm18, xmm27, xmm18
	vpsrlvq ymm29, ymm17, ymm26
	vpsrlvq xmm18, xmm16, xmm20
	vpsrlvq ymm29, ymm31, ymm21
	vpsrlvq xmm0{k2}, xmm5, xmm0
	vpsrlvq ymm2{k4}, ymm6, ymm1
	vpsrlvq zmm0{k6}, zmm2, zmm1
	vpsrlvq xmm7{k3}{z}, xmm0, xmm2
	vpsrlvq ymm5{k1}{z}, ymm5, ymm2
	vpsrlvq zmm2{k7}{z}, zmm3, zmm0
	vpsrlvq xmm1, xmm5, oword [eax+1]
	vpsrlvq xmm1, xmm5, oword [eax+64]
	vpsrlvq ymm0, ymm0, yword [eax+1]
	vpsrlvq ymm3, ymm0, yword [eax+64]
	vpsrlvq xmm4, xmm2, oword [eax+1]
	vpsrlvq xmm7, xmm6, oword [eax+64]
	vpsrlvq ymm1, ymm4, yword [eax+1]
	vpsrlvq ymm4, ymm1, yword [eax+64]
	vpsrlvq zmm6, zmm6, zword [eax+1]
	vpsrlvq zmm5, zmm4, zword [eax+64]
	vpsrlvq xmm0, xmm6, [0x91b]
	vpsrlvq ymm7, ymm4, [0x7bd]
	vpsrlvq xmm4, xmm5, [0x769]
	vpsrlvq ymm2, ymm1, [0xf9b]
	vpsrlvq zmm4, zmm6, [0x534]

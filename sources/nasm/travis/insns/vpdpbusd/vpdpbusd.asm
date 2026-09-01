default rel
	vpdpbusd xmm7, xmm6, xmm1
	vpdpbusd xmm0, xmm4, xmm3
	vpdpbusd ymm3, ymm1, ymm4
	vpdpbusd ymm2, ymm7, ymm0
	vpdpbusd xmm2, xmm2, xmm7
	vpdpbusd xmm2, xmm7
	vpdpbusd xmm1, xmm2, xmm3
	vpdpbusd ymm6, ymm0, ymm6
	vpdpbusd ymm6, ymm6
	vpdpbusd ymm7, ymm1, ymm3
	vpdpbusd xmm11, xmm8, xmm8
	vpdpbusd ymm9, ymm10, ymm11
	vpdpbusd xmm15, xmm9, xmm8
	vpdpbusd ymm15, ymm11, ymm15
	vpdpbusd xmm26, xmm18, xmm30
	vpdpbusd ymm19, ymm16, ymm20
	vpdpbusd xmm25, xmm27, xmm18
	vpdpbusd ymm31, ymm29, ymm21
	vpdpbusd xmm2{k5}, xmm6, xmm3
	vpdpbusd ymm0{k2}, ymm0, ymm7
	vpdpbusd zmm3{k3}, zmm4, zword [0x2bd]
	vpdpbusd xmm3{k6}{z}, xmm0, xmm6
	vpdpbusd ymm0{k1}{z}, ymm5, yword [0x108]
	vpdpbusd zmm0{k5}{z}, zmm6, zmm2
	vpdpbusd xmm5, xmm6, oword [eax+1]
	vpdpbusd xmm4, xmm5, oword [eax+64]
	vpdpbusd ymm6, ymm5, yword [eax+1]
	vpdpbusd ymm7, ymm7, yword [eax+64]
	vpdpbusd xmm7, xmm6, oword [eax+1]
	vpdpbusd xmm4, xmm7, oword [eax+64]
	vpdpbusd ymm0, ymm5, yword [eax+1]
	vpdpbusd ymm0, ymm2, yword [eax+64]
	vpdpbusd zmm6, zmm5, zword [eax+1]
	vpdpbusd zmm4, zmm7, zword [eax+64]
	vpdpbusd xmm1, xmm6, [0xc78]
	vpdpbusd ymm5, ymm4, [0xbd6]
	vpdpbusd xmm6, xmm7, [0xc24]
	vpdpbusd ymm3, ymm6, [0x7cd]
	vpdpbusd zmm7, zmm3, [0xa0b]

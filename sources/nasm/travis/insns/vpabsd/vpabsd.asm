default rel
	vpabsd xmm3, oword [0xfec]
	vpabsd xmm1, xmm4
	vpabsd ymm1, ymm6
	vpabsd ymm4, ymm2
	vpabsd xmm5, oword [0xc6c]
	vpabsd xmm0, xmm0
	vpabsd ymm0, ymm3
	vpabsd ymm4, ymm1
	vpabsd xmm11, xmm14
	vpabsd ymm12, ymm10
	vpabsd xmm13, xmm15
	vpabsd ymm12, ymm9
	vpabsd xmm22, xmm22
	vpabsd ymm16, ymm24
	vpabsd xmm22, xmm26
	vpabsd ymm20, ymm17
	vpabsd xmm6{k1}, oword [0x1c0]
	vpabsd ymm7{k3}, ymm2
	vpabsd zmm2{k5}, zmm1
	vpabsd xmm1{k2}{z}, oword [0x9df]
	vpabsd ymm1{k4}{z}, ymm6
	vpabsd zmm5{k6}{z}, zword [0x243]
	vpabsd xmm2, oword [eax+1]
	vpabsd xmm7, oword [eax+64]
	vpabsd ymm3, yword [eax+1]
	vpabsd ymm2, yword [eax+64]
	vpabsd xmm6, oword [eax+1]
	vpabsd xmm1, oword [eax+64]
	vpabsd ymm4, yword [eax+1]
	vpabsd ymm1, yword [eax+64]
	vpabsd zmm0, zword [eax+1]
	vpabsd zmm7, zword [eax+64]
	vpabsd xmm5, [0xc42]
	vpabsd ymm2, [0xead]
	vpabsd xmm3, [0xa74]
	vpabsd ymm5, [0x804]
	vpabsd zmm5, [0x528]

default rel
	vscalefpd xmm6, xmm0, xmm7
	vscalefpd xmm6, xmm7
	vscalefpd xmm2, xmm7, xmm3
	vscalefpd ymm4, ymm0, ymm5
	vscalefpd ymm4, ymm5
	vscalefpd ymm1, ymm3, ymm7
	vscalefpd zmm1, zmm3, zmm1
	vscalefpd zmm1, zmm1
	vscalefpd zmm2, zmm4, zword [0x396]
	vscalefpd xmm11, xmm10, xmm8
	vscalefpd ymm13, ymm15, ymm10
	vscalefpd zmm8, zmm12, zmm13
	vscalefpd xmm22, xmm22, xmm22
	vscalefpd ymm24, ymm20, ymm26
	vscalefpd zmm27, zmm18, zmm24
	vscalefpd xmm3{k7}, xmm7, xmm4
	vscalefpd ymm1{k1}, ymm2, ymm7
	vscalefpd zmm2{k7}, zmm5, zword [0x8f1]
	vscalefpd xmm0{k2}{z}, xmm3, xmm1
	vscalefpd ymm3{k5}{z}, ymm5, ymm3
	vscalefpd zmm3{k4}{z}, zmm1, zmm7
	vscalefpd zmm0, zmm4, zmm5, {rn-sae}
	vscalefpd xmm0, xmm5, oword [eax+1]
	vscalefpd xmm4, xmm7, oword [eax+64]
	vscalefpd ymm7, ymm5, yword [eax+1]
	vscalefpd ymm1, ymm7, yword [eax+64]
	vscalefpd zmm1, zmm6, zword [eax+1]
	vscalefpd zmm3, zmm5, zword [eax+64]
	vscalefpd xmm5, xmm2, [0x510]
	vscalefpd ymm6, ymm4, [0x6c0]
	vscalefpd zmm2, zmm3, [0xd97]

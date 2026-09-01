default rel
	vfmsubpd xmm4, xmm5, xmm0, xmm1
	vfmsubpd xmm4, xmm0, xmm1
	vfmsubpd xmm6, xmm0, oword [0x9bb], xmm6
	vfmsubpd ymm2, ymm7, ymm5, ymm7
	vfmsubpd ymm2, ymm5, ymm7
	vfmsubpd ymm4, ymm7, yword [0x799], ymm0
	vfmsubpd xmm7, xmm4, xmm1, oword [0x6e5]
	vfmsubpd xmm7, xmm1, oword [0x6e5]
	vfmsubpd xmm2, xmm0, xmm3, xmm3
	vfmsubpd ymm7, ymm6, ymm4, ymm7
	vfmsubpd ymm7, ymm4, ymm7
	vfmsubpd ymm4, ymm7, ymm4, ymm0
	vfmsubpd xmm12, xmm9, xmm15, xmm8
	vfmsubpd ymm14, ymm14, ymm14, ymm8
	vfmsubpd xmm8, xmm8, xmm10, xmm8
	vfmsubpd ymm10, ymm13, ymm13, ymm13
	vfmsubpd xmm7, xmm0, oword [eax+1], xmm4
	vfmsubpd xmm7, xmm5, oword [eax+64], xmm5
	vfmsubpd ymm6, ymm7, yword [eax+1], ymm4
	vfmsubpd ymm0, ymm0, yword [eax+64], ymm5
	vfmsubpd xmm3, xmm1, xmm5, oword [eax+1]
	vfmsubpd xmm2, xmm6, xmm7, oword [eax+64]
	vfmsubpd ymm7, ymm4, ymm5, yword [eax+1]
	vfmsubpd ymm6, ymm7, ymm4, yword [eax+64]
	vfmsubpd xmm2, xmm3, [0x4fd], xmm7
	vfmsubpd ymm1, ymm6, [0x4e2], ymm1
	vfmsubpd xmm1, xmm4, xmm0, [0xc81]
	vfmsubpd ymm3, ymm4, ymm4, [0x961]

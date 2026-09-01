default rel
	vfmaddsubpd xmm0, xmm7, oword [0x813], xmm2
	vfmaddsubpd xmm0, oword [0x813], xmm2
	vfmaddsubpd xmm6, xmm5, xmm2, xmm4
	vfmaddsubpd ymm5, ymm0, ymm6, ymm2
	vfmaddsubpd ymm5, ymm6, ymm2
	vfmaddsubpd ymm2, ymm5, ymm4, ymm2
	vfmaddsubpd xmm0, xmm2, xmm5, xmm3
	vfmaddsubpd xmm0, xmm5, xmm3
	vfmaddsubpd xmm7, xmm1, xmm4, xmm2
	vfmaddsubpd ymm3, ymm0, ymm6, ymm6
	vfmaddsubpd ymm3, ymm6, ymm6
	vfmaddsubpd ymm6, ymm4, ymm4, ymm5
	vfmaddsubpd xmm10, xmm12, xmm8, xmm9
	vfmaddsubpd ymm15, ymm11, ymm15, ymm10
	vfmaddsubpd xmm14, xmm12, xmm11, xmm8
	vfmaddsubpd ymm14, ymm14, ymm10, ymm9
	vfmaddsubpd xmm5, xmm6, oword [eax+1], xmm3
	vfmaddsubpd xmm1, xmm6, oword [eax+64], xmm7
	vfmaddsubpd ymm4, ymm4, yword [eax+1], ymm4
	vfmaddsubpd ymm6, ymm4, yword [eax+64], ymm2
	vfmaddsubpd xmm0, xmm6, xmm4, oword [eax+1]
	vfmaddsubpd xmm6, xmm1, xmm4, oword [eax+64]
	vfmaddsubpd ymm5, ymm4, ymm0, yword [eax+1]
	vfmaddsubpd ymm6, ymm2, ymm3, yword [eax+64]
	vfmaddsubpd xmm4, xmm0, [0x85a], xmm7
	vfmaddsubpd ymm2, ymm0, [0xa3c], ymm6
	vfmaddsubpd xmm6, xmm4, xmm3, [0xac4]
	vfmaddsubpd ymm6, ymm3, ymm7, [0xbf7]

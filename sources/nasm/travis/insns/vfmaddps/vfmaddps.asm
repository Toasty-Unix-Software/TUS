default rel
	vfmaddps xmm6, xmm0, oword [0x204], xmm6
	vfmaddps xmm6, oword [0x204], xmm6
	vfmaddps xmm1, xmm4, xmm6, xmm7
	vfmaddps ymm4, ymm0, ymm0, ymm7
	vfmaddps ymm4, ymm0, ymm7
	vfmaddps ymm2, ymm1, yword [0xfb7], ymm4
	vfmaddps xmm4, xmm4, xmm6, xmm4
	vfmaddps xmm4, xmm6, xmm4
	vfmaddps xmm2, xmm7, xmm3, xmm2
	vfmaddps ymm6, ymm6, ymm3, ymm4
	vfmaddps ymm6, ymm3, ymm4
	vfmaddps ymm6, ymm5, ymm2, ymm2
	vfmaddps xmm14, xmm8, xmm9, xmm12
	vfmaddps ymm13, ymm8, ymm13, ymm14
	vfmaddps xmm12, xmm9, xmm14, xmm14
	vfmaddps ymm9, ymm12, ymm13, ymm9
	vfmaddps xmm6, xmm3, oword [eax+1], xmm7
	vfmaddps xmm6, xmm5, oword [eax+64], xmm2
	vfmaddps ymm2, ymm3, yword [eax+1], ymm4
	vfmaddps ymm2, ymm7, yword [eax+64], ymm0
	vfmaddps xmm7, xmm6, xmm2, oword [eax+1]
	vfmaddps xmm5, xmm2, xmm5, oword [eax+64]
	vfmaddps ymm4, ymm5, ymm7, yword [eax+1]
	vfmaddps ymm7, ymm0, ymm0, yword [eax+64]
	vfmaddps xmm4, xmm6, [0xef7], xmm3
	vfmaddps ymm4, ymm2, [0xc02], ymm4
	vfmaddps xmm5, xmm1, xmm1, [0xd5e]
	vfmaddps ymm0, ymm7, ymm6, [0x5bb]

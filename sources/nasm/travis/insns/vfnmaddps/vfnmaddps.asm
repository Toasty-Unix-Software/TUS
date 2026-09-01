default rel
	vfnmaddps xmm1, xmm5, oword [0xcae], xmm3
	vfnmaddps xmm1, oword [0xcae], xmm3
	vfnmaddps xmm4, xmm5, xmm3, xmm5
	vfnmaddps ymm1, ymm2, yword [0x90f], ymm2
	vfnmaddps ymm1, yword [0x90f], ymm2
	vfnmaddps ymm0, ymm3, ymm5, ymm1
	vfnmaddps xmm1, xmm0, xmm6, xmm3
	vfnmaddps xmm1, xmm6, xmm3
	vfnmaddps xmm0, xmm5, xmm5, oword [0x658]
	vfnmaddps ymm1, ymm2, ymm6, ymm3
	vfnmaddps ymm1, ymm6, ymm3
	vfnmaddps ymm1, ymm1, ymm2, yword [0xf7e]
	vfnmaddps xmm9, xmm9, xmm8, xmm10
	vfnmaddps ymm9, ymm10, ymm9, ymm12
	vfnmaddps xmm13, xmm11, xmm9, xmm9
	vfnmaddps ymm15, ymm11, ymm9, ymm15
	vfnmaddps xmm4, xmm2, oword [eax+1], xmm3
	vfnmaddps xmm6, xmm7, oword [eax+64], xmm6
	vfnmaddps ymm4, ymm1, yword [eax+1], ymm6
	vfnmaddps ymm7, ymm5, yword [eax+64], ymm6
	vfnmaddps xmm7, xmm0, xmm3, oword [eax+1]
	vfnmaddps xmm7, xmm4, xmm4, oword [eax+64]
	vfnmaddps ymm4, ymm7, ymm5, yword [eax+1]
	vfnmaddps ymm3, ymm3, ymm3, yword [eax+64]
	vfnmaddps xmm5, xmm6, [0xdc5], xmm5
	vfnmaddps ymm3, ymm5, [0xfcd], ymm4
	vfnmaddps xmm2, xmm3, xmm2, [0xabb]
	vfnmaddps ymm1, ymm4, ymm1, [0xca1]

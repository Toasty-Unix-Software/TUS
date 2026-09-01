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

%ifdef ERROR
	vfnmaddps xmm9, xmm9, xmm8, xmm10
	vfnmaddps ymm9, ymm10, ymm9, ymm12
	vfnmaddps xmm13, xmm11, xmm9, xmm9
	vfnmaddps ymm15, ymm11, ymm9, ymm15
%endif

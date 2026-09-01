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

%ifdef ERROR
	vfmaddps xmm14, xmm8, xmm9, xmm12
	vfmaddps ymm13, ymm8, ymm13, ymm14
	vfmaddps xmm12, xmm9, xmm14, xmm14
	vfmaddps ymm9, ymm12, ymm13, ymm9
%endif

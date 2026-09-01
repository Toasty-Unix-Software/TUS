	vfmaddpd xmm6, xmm0, oword [0x3e7], xmm4
	vfmaddpd xmm6, oword [0x3e7], xmm4
	vfmaddpd xmm1, xmm1, xmm6, xmm3
	vfmaddpd ymm3, ymm7, yword [0x543], ymm4
	vfmaddpd ymm3, yword [0x543], ymm4
	vfmaddpd ymm6, ymm7, ymm5, ymm2
	vfmaddpd xmm7, xmm7, xmm6, oword [0x3b5]
	vfmaddpd xmm7, xmm6, oword [0x3b5]
	vfmaddpd xmm6, xmm7, xmm7, xmm7
	vfmaddpd ymm2, ymm1, ymm1, ymm6
	vfmaddpd ymm2, ymm1, ymm6
	vfmaddpd ymm0, ymm7, ymm5, ymm5

%ifdef ERROR
	vfmaddpd xmm12, xmm15, xmm14, xmm12
	vfmaddpd ymm9, ymm14, ymm8, ymm12
	vfmaddpd xmm14, xmm13, xmm8, xmm9
	vfmaddpd ymm13, ymm10, ymm14, ymm12
%endif

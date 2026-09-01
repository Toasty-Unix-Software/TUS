	vfnmaddpd xmm5, xmm7, xmm6, xmm3
	vfnmaddpd xmm5, xmm6, xmm3
	vfnmaddpd xmm2, xmm4, xmm4, xmm6
	vfnmaddpd ymm7, ymm6, yword [0x8b1], ymm7
	vfnmaddpd ymm7, yword [0x8b1], ymm7
	vfnmaddpd ymm7, ymm5, ymm2, ymm0
	vfnmaddpd xmm4, xmm0, xmm6, xmm5
	vfnmaddpd xmm4, xmm6, xmm5
	vfnmaddpd xmm6, xmm2, xmm3, oword [0xd0c]
	vfnmaddpd ymm7, ymm2, ymm5, ymm5
	vfnmaddpd ymm7, ymm5, ymm5
	vfnmaddpd ymm0, ymm2, ymm1, ymm4

%ifdef ERROR
	vfnmaddpd xmm11, xmm11, xmm13, xmm15
	vfnmaddpd ymm11, ymm9, ymm8, ymm9
	vfnmaddpd xmm15, xmm11, xmm15, xmm10
	vfnmaddpd ymm12, ymm8, ymm13, ymm12
%endif

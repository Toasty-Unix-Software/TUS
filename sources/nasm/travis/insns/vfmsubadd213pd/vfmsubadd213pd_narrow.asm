	vfmsubadd213pd xmm4, xmm1, xmm7
	vfmsubadd213pd xmm0, xmm6, oword [0x127]
	vfmsubadd213pd ymm4, ymm2, yword [0x6bb]
	vfmsubadd213pd ymm5, ymm0, yword [0xa53]
	vfmsubadd213pd xmm1, xmm1, xmm0
	vfmsubadd213pd xmm1, xmm0, xmm5
	vfmsubadd213pd ymm0, ymm4, ymm3
	vfmsubadd213pd ymm5, ymm4, ymm7

%ifdef ERROR
	vfmsubadd213pd xmm8, xmm11, xmm12
	vfmsubadd213pd ymm10, ymm10, ymm14
	vfmsubadd213pd xmm12, xmm10, xmm11
	vfmsubadd213pd ymm15, ymm15, ymm13
	vfmsubadd213pd xmm17, xmm24, xmm21
	vfmsubadd213pd ymm27, ymm26, ymm27
	vfmsubadd213pd xmm27, xmm16, xmm31
	vfmsubadd213pd ymm27, ymm29, ymm24
%endif

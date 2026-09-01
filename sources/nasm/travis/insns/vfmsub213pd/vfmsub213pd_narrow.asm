	vfmsub213pd xmm6, xmm0, xmm2
	vfmsub213pd xmm5, xmm7, xmm1
	vfmsub213pd ymm2, ymm4, ymm3
	vfmsub213pd ymm2, ymm2, ymm1
	vfmsub213pd xmm1, xmm0, oword [0x260]
	vfmsub213pd xmm2, xmm0, xmm2
	vfmsub213pd ymm3, ymm1, yword [0x7d0]
	vfmsub213pd ymm2, ymm5, ymm3

%ifdef ERROR
	vfmsub213pd xmm10, xmm9, xmm15
	vfmsub213pd ymm9, ymm13, ymm14
	vfmsub213pd xmm14, xmm11, xmm9
	vfmsub213pd ymm13, ymm15, ymm12
	vfmsub213pd xmm16, xmm31, xmm16
	vfmsub213pd ymm17, ymm24, ymm23
	vfmsub213pd xmm30, xmm27, xmm19
	vfmsub213pd ymm30, ymm29, ymm16
%endif

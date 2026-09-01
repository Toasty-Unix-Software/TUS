	vfmsub132pd xmm1, xmm7, xmm3
	vfmsub132pd xmm0, xmm3, xmm2
	vfmsub132pd ymm7, ymm3, ymm7
	vfmsub132pd ymm1, ymm5, ymm3
	vfmsub132pd xmm5, xmm1, oword [0x6bb]
	vfmsub132pd xmm0, xmm5, xmm7
	vfmsub132pd ymm5, ymm5, ymm7
	vfmsub132pd ymm1, ymm7, yword [0x283]

%ifdef ERROR
	vfmsub132pd xmm10, xmm10, xmm13
	vfmsub132pd ymm8, ymm8, ymm9
	vfmsub132pd xmm13, xmm10, xmm10
	vfmsub132pd ymm15, ymm12, ymm11
	vfmsub132pd xmm19, xmm18, xmm26
	vfmsub132pd ymm29, ymm16, ymm16
	vfmsub132pd xmm22, xmm20, xmm19
	vfmsub132pd ymm19, ymm17, ymm31
%endif

	vfmsub213ss xmm0, xmm6, xmm0
	vfmsub213ss xmm5, xmm3, xmm2
	vfmsub213ss xmm4, xmm6, xmm1
	vfmsub213ss xmm7, xmm4, xmm7

%ifdef ERROR
	vfmsub213ss xmm12, xmm14, xmm14
	vfmsub213ss xmm10, xmm9, xmm9
	vfmsub213ss xmm23, xmm26, xmm19
	vfmsub213ss xmm30, xmm20, xmm22
%endif

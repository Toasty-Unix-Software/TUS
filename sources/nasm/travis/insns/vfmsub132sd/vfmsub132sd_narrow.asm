	vfmsub132sd xmm1, xmm2, xmm6
	vfmsub132sd xmm0, xmm7, xmm7
	vfmsub132sd xmm5, xmm5, xmm2
	vfmsub132sd xmm0, xmm7, xmm3

%ifdef ERROR
	vfmsub132sd xmm10, xmm9, xmm10
	vfmsub132sd xmm10, xmm11, xmm8
	vfmsub132sd xmm21, xmm18, xmm19
	vfmsub132sd xmm23, xmm20, xmm28
%endif

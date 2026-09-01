	vfnmsub213sh xmm4, xmm3, xmm3
	vfnmsub213sh xmm4, xmm3
	vfnmsub213sh xmm4, xmm4, xmm2

%ifdef ERROR
	vfnmsub213sh xmm8, xmm14, xmm12
	vfnmsub213sh xmm24, xmm31, xmm24
%endif

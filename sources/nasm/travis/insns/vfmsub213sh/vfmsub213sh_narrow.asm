	vfmsub213sh xmm4, xmm7, word [0xa6f]
	vfmsub213sh xmm4, word [0xa6f]
	vfmsub213sh xmm6, xmm7, xmm3

%ifdef ERROR
	vfmsub213sh xmm15, xmm10, xmm8
	vfmsub213sh xmm27, xmm21, xmm29
%endif

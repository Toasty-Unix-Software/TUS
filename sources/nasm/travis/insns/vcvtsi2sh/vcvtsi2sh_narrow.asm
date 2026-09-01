	vcvtsi2sh xmm0, xmm5, eax
	vcvtsi2sh xmm0, eax
	vcvtsi2sh xmm1, xmm7, eax
	vcvtsi2sh xmm1, xmm4, rcx
	vcvtsi2sh xmm1, rcx
	vcvtsi2sh xmm5, xmm7, rdx

%ifdef ERROR
	vcvtsi2sh xmm12, xmm12, r9d
	vcvtsi2sh xmm11, xmm15, r8
	vcvtsi2sh xmm26, xmm19, r22d
	vcvtsi2sh xmm18, xmm20, r22
%endif

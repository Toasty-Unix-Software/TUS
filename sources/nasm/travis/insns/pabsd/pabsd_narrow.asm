	pabsd mm0, mm5
	pabsd mm0, mm0
	pabsd xmm1, xmm0
	pabsd xmm3, xmm4

%ifdef ERROR
	pabsd xmm12, xmm8
%endif

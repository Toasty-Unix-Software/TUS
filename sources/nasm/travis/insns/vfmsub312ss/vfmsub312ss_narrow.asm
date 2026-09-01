	vfmsub312ss xmm2, xmm6, dword [0x4db]
	vfmsub312ss xmm0, xmm1, xmm7

%ifdef ERROR
	vfmsub312ss xmm11, xmm8, xmm10
%endif

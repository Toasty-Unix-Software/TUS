default rel
	vcmpeq_ussd xmm2, xmm3, qword [0x9c1]
	vcmpeq_ussd xmm2, qword [0x9c1]
	vcmpeq_ussd xmm5, xmm5, xmm3
	vcmpeq_ussd k7, xmm1, xmm4
	vcmpeq_ussd k2, xmm6, qword [0x150]
	vcmpeq_ussd xmm15, xmm10, xmm9
	vcmpeq_ussd k4, xmm13, xmm15
	vcmpeq_ussd k4{k7}, xmm2, xmm3
	vcmpeq_ussd k4, xmm4, xmm7, {sae}
	vcmpeq_ussd xmm3, xmm7, qword [eax+1]
	vcmpeq_ussd xmm4, xmm1, qword [eax+64]
	vcmpeq_ussd k4, xmm2, qword [eax+1]
	vcmpeq_ussd k3, xmm6, qword [eax+64]
	vcmpeq_ussd xmm1, xmm1, [0x193]
	vcmpeq_ussd k5, xmm6, [0x9db]

default rel
	vmovsd xmm2, xmm4, xmm3
	vmovsd xmm2, xmm3
	vmovsd xmm3, xmm7, xmm1
	vmovsd xmm2, qword [0xd8d]
	vmovsd xmm4, qword [0x40c]
	vmovsd qword [0xf79], xmm5
	vmovsd qword [0x5d5], xmm4
	vmovsd xmm0, qword [0x758]
	vmovsd xmm2, qword [0x184]
	vmovsd xmm12, xmm15, xmm12
	vmovsd xmm14, qword [0x7cf]
	vmovsd qword [0xe48], xmm14
	vmovsd xmm15, qword [0xe16]
	vmovsd xmm29, xmm31, xmm17
	vmovsd xmm28, qword [0x921]
	vmovsd qword [0xc9a], xmm27
	vmovsd xmm19, qword [0x47c]
	vmovsd xmm5{k4}, qword [0xf4f]
	vmovsd qword [0xd55]{k3}, xmm7
	vmovsd xmm3{k5}, xmm3, xmm2
	vmovsd xmm4, qword [eax+1]
	vmovsd xmm4, qword [eax+64]
	vmovsd qword [eax+1], xmm4
	vmovsd qword [eax+64], xmm3
	vmovsd xmm7, qword [eax+1]
	vmovsd xmm0, qword [eax+64]
	vmovsd qword [eax+1], xmm1
	vmovsd qword [eax+64], xmm1
	vmovsd xmm4, [0xdec]
	vmovsd [0xb27], xmm3
	vmovsd xmm5, [0x92b]
	vmovsd [0x12e], xmm5

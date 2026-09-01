default rel
	vcvtsi2ss xmm4, xmm7, ebx
	vcvtsi2ss xmm4, ebx
	vcvtsi2ss xmm3, xmm3, edx
	vcvtsi2ss xmm4, xmm6, dword [0x6f6]
	vcvtsi2ss xmm4, dword [0x6f6]
	vcvtsi2ss xmm5, xmm6, dword [0x670]
	vcvtsi2ss xmm5, xmm4, qword [0x91c]
	vcvtsi2ss xmm5, qword [0x91c]
	vcvtsi2ss xmm1, xmm3, rdx
	vcvtsi2ss xmm6, xmm6, dword [0x462]
	vcvtsi2ss xmm6, dword [0x462]
	vcvtsi2ss xmm6, xmm4, eax
	vcvtsi2ss xmm10, xmm8, r15d
	vcvtsi2ss xmm10, xmm9, dword [0xcff]
	vcvtsi2ss xmm12, xmm13, r14
	vcvtsi2ss xmm9, xmm8, r9d
	vcvtsi2ss xmm28, xmm17, r28d
	vcvtsi2ss xmm18, xmm26, dword [0xf6d]
	vcvtsi2ss xmm23, xmm16, r16
	vcvtsi2ss xmm18, xmm29, r20d
	vcvtsi2ss xmm6, xmm3, eax, {rd-sae}
	vcvtsi2ss xmm4, xmm0, rdx, {ru-sae}
	vcvtsi2ss xmm3, xmm6, dword [eax+1]
	vcvtsi2ss xmm0, xmm5, dword [eax+64]
	vcvtsi2ss xmm7, xmm4, dword [eax+1]
	vcvtsi2ss xmm0, xmm0, dword [eax+64]
	vcvtsi2ss xmm2, xmm2, qword [eax+1]
	vcvtsi2ss xmm2, xmm2, qword [eax+64]
	vcvtsi2ss xmm7, xmm3, dword [eax+1]
	vcvtsi2ss xmm7, xmm0, dword [eax+64]
	vcvtsi2ss xmm0, xmm5, qword [eax+1]
	vcvtsi2ss xmm7, xmm0, qword [eax+64]
	vcvtsi2ss xmm6, xmm5, [0x9af]
	vcvtsi2ss xmm0, xmm1, [0xba0]
	vcvtsi2ss xmm6, xmm1, [0x39b]
	vcvtsi2ss xmm5, xmm2, [0x7d2]
	vcvtsi2ss xmm7, xmm4, [0x560]

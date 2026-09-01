default rel
	pmaxsw mm4, mm0
	pmaxsw mm1, mm6
	pmaxsw xmm3, xmm5
	pmaxsw xmm2, xmm1
	pmaxsw xmm10, xmm10
	pmaxsw mm6, qword [eax+1]
	pmaxsw mm6, qword [eax+64]
	pmaxsw xmm2, oword [eax+1]
	pmaxsw xmm4, oword [eax+64]
	pmaxsw mm3, [0x8db]
	pmaxsw xmm0, [0x7ff]

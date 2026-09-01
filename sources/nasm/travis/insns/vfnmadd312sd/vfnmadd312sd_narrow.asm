	vfnmadd312sd xmm4, xmm4, xmm5
	vfnmadd312sd xmm2, xmm6, qword [0x4db]

%ifdef ERROR
	vfnmadd312sd xmm12, xmm13, xmm8
%endif

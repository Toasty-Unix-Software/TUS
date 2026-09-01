	bndldx bnd2, [0x4ec]
	bndldx bnd0, [0x6f6]
	bndldx bnd0, [0x110], ebx
	bndldx bnd1, [0xc61], ebp

%ifdef ERROR
	bndldx bnd0, [0xc56], rdx
	bndldx bnd0, [0x2c1], rcx
%endif

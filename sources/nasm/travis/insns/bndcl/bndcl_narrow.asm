	bndcl bnd0, [0xa79]
	bndcl bnd1, [0x351]
	bndcl bnd2, ebx
	bndcl bnd0, edi

%ifdef ERROR
	bndcl bnd3, rdx
	bndcl bnd0, rbx
%endif

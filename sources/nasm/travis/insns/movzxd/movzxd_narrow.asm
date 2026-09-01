	movzxd cx, ebx
	movzxd dx, ebp
	movzxd ebx, eax
	movzxd ebp, ebp

%ifdef ERROR
	movzxd rax, ebx
	movzxd rbx, esi
%endif

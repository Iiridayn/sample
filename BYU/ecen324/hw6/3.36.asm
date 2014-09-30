
3.36.o:     file format elf32-i386

Disassembly of section .text:

00000000 <test>:
   0:	55                   	push   %ebp
   1:	89 e5                	mov    %esp,%ebp
   3:	8b 45 08             	mov    0x8(%ebp),%eax
   6:	8b 55 0c             	mov    0xc(%ebp),%edx
   9:	8b 0a                	mov    (%edx),%ecx
   b:	03 8a b8 00 00 00    	add    0xb8(%edx),%ecx
  11:	8d 04 80             	lea    (%eax,%eax,4),%eax
  14:	8d 44 82 04          	lea    0x4(%edx,%eax,4),%eax
  18:	8b 10                	mov    (%eax),%edx
  1a:	89 4c 90 04          	mov    %ecx,0x4(%eax,%edx,4)
  1e:	5d                   	pop    %ebp
  1f:	c3                   	ret    
Disassembly of section .comment:

00000000 <.comment>:
   0:	00 47 43             	add    %al,0x43(%edi)
   3:	43                   	inc    %ebx
   4:	3a 20                	cmp    (%eax),%ah
   6:	28 47 4e             	sub    %al,0x4e(%edi)
   9:	55                   	push   %ebp
   a:	29 20                	sub    %esp,(%eax)
   c:	33 2e                	xor    (%esi),%ebp
   e:	32 2e                	xor    (%esi),%ch
  10:	33 20                	xor    (%eax),%esp
  12:	32 30                	xor    (%eax),%dh
  14:	30 33                	xor    %dh,(%ebx)
  16:	30 34 32             	xor    %dh,(%edx,%esi,1)
  19:	32 20                	xor    (%eax),%ah
  1b:	28 47 65             	sub    %al,0x65(%edi)
  1e:	6e                   	outsb  %ds:(%esi),(%dx)
  1f:	74 6f                	je     90 <test+0x90>
  21:	6f                   	outsl  %ds:(%esi),(%dx)
  22:	20 4c 69 6e          	and    %cl,0x6e(%ecx,%ebp,2)
  26:	75 78                	jne    a0 <test+0xa0>
  28:	20 31                	and    %dh,(%ecx)
  2a:	2e                   	cs
  2b:	34 20                	xor    $0x20,%al
  2d:	33 2e                	xor    (%esi),%ebp
  2f:	32 2e                	xor    (%esi),%ch
  31:	33 2d 72 31 2c 20    	xor    0x202c3172,%ebp
  37:	70 72                	jo     ab <test+0xab>
  39:	6f                   	outsl  %ds:(%esi),(%dx)
  3a:	70 6f                	jo     ab <test+0xab>
  3c:	6c                   	insb   (%dx),%es:(%edi)
  3d:	69                   	.byte 0x69
  3e:	63 65 29             	arpl   %sp,0x29(%ebp)
	...

#pragma once

enum VertexType
{
	POSITION,
	NORMAL,
	COLOR
};

struct VertexAttribute
{
	int Index;
	int Size;
	int Type;
	bool Normalized;
	int Stride;
	int Pointer;

	VertexAttribute(int Index, int Size, int Type, bool Normalized, int Stride, int Pointer)
		: Index(Index), Size(Size), Type(Type), Normalized(Normalized), Stride(Stride), Pointer(Pointer) { }

};
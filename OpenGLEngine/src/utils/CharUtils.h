#pragma once

#include <stdexcept>

class CharUtils
{

public:

	/**
	* Set bits in a byte to a value, using the location of the starting bit (0 - 7)
	* and the number of bits that is needed to be set.
	*
	* @param b
	* @param bitIndex
	* @param bitLength
	* @param val
	* @return
	*/
	static void SetCharBits(char& b, int bitIndex, int bitLength, int val)
	{
		if (bitIndex > 7 || bitIndex < 0)
			throw std::invalid_argument("CharUtils: index is invalid.");
		if (bitIndex + bitLength > 8)
			throw std::invalid_argument("CharUtils: index and bit length is out of bound.");

		char maxValue = (1 << bitLength) - 1;
		if (val > maxValue)
			throw std::invalid_argument("CharUtils: value is too big for bits length");

		char byteAnd = (char)~(maxValue << bitIndex);

		b = ((b & byteAnd) | (val << bitIndex));
	}

	/**
	* Get bits in a byte as an int value, using the location of the starting
	* bit (0 - 7) and the number of bits that is needed.
	*
	* @param b
	* @param bitIndex
	* @param bitLength
	* @return
	*/
	static int GetCharBits(char b, int bitIndex, int bitLength)
	{
		if (bitIndex > 7 || bitIndex < 0)
			throw std::invalid_argument("CharUtils: index is invalid.");
		if (bitIndex + bitLength > 8)
			throw std::invalid_argument("CharUtils: index and bit length is out of bound.");

		return (b >> bitIndex) & ((1 << bitLength) - 1);
	}

	/**
	* Set the bit at index to 1.
	*
	* @param b
	* @param bitIndex
	* @return
	*/
	static void SetBit(char& b, int bitIndex)
	{
		if (bitIndex > 7 || bitIndex < 0)
			throw std::invalid_argument("CharUtils: index is invalid.");
		b |= (1 << bitIndex);
	}

	/**
	* Set the bit at index to 0.
	*
	* @param b
	* @param bitIndex
	* @return
	*/
	static void UnsetBit(char& b, int bitIndex)
	{
		if (bitIndex > 7 || bitIndex < 0)
			throw std::invalid_argument("CharUtils: index is invalid.");
		b &= ~(1 << bitIndex);
	}

	/**
	* Toggle the bit at index.
	*
	* @param b
	* @param bitIndex
	* @return
	*/
	static void ToggleBit(char& b, int bitIndex)
	{
		if (bitIndex > 7 || bitIndex < 0)
			throw std::invalid_argument("CharUtils: index is invalid.");
		b ^= (1 << bitIndex);
	}

	/**
	* Get the value of a bit (0 or 1) at index.
	*
	* @param b
	* @param bitIndex
	* @return
	*/
	static char GetBit(char b, int bitIndex)
	{
		if (bitIndex > 7 || bitIndex < 0)
			throw std::invalid_argument("CharUtils: index is invalid.");
		return (b >> bitIndex) & 1;
	}

};
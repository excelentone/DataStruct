#pragma once

struct TrueType
{};

struct FalseType
{};

template<class T>
struct TypeTraits
{
	typedef FalseType    hasTrivialDefaultConstructor;
	typedef FalseType    hasTrivialCopyConstructor;
	typedef FalseType    hasTrivialAssignmentOperator;
	typedef FalseType    hasTrivialDestructor;
	typedef FalseType    isPODType;
};


template<>
struct TypeTraits<int>
{
	typedef TrueType    hasTrivialDefaultConstructor;
	typedef TrueType    hasTrivialCopyConstructor;
	typedef TrueType    hasTrivialAssignmentOperator;
	typedef TrueType    hasTrivialDestructor;
	typedef TrueType    isPODType;
};

template<>
struct TypeTraits<char>
{
	typedef TrueType    hasTrivialDefaultConstructor;
	typedef TrueType    hasTrivialCopyConstructor;
	typedef TrueType    hasTrivialAssignmentOperator;
	typedef TrueType    hasTrivialDestructor;
	typedef TrueType    isPODType;
};








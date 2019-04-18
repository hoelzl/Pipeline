#include <functional>
#include <iostream>
#include <string>

#include "FunctionalPipeline.h"
#include "Pipeline.h"

// Some functions that we want to put into the pipelines

int AddOne(int InArg)
{
	std::cerr << "  ... in AddOne()" << std::endl;
	return InArg + 1;
}

std::string ToString(int InNumber)
{
	std::cerr << "  ... in ToString(int)" << std::endl;
	return std::to_string(InNumber);
}

// Non-overloaded version of ToString
std::string IntToString(int InNumber)
{
	std::cerr << "  ... in IntToString()" << std::endl;
	return std::to_string(InNumber);
}

std::string Emphasize(std::string InString)
{
	std::cerr << "  ... in Emphasize()" << std::endl;
	return "*" + InString + "*";
}

int Length(std::string InString)
{
	return InString.length();
}

struct AnnotatedString
{
	std::string String;
	std::string Annotation;
};

// Funcallable class
struct Annotator
{
	Annotator(std::string InAnnotation) : Annotation{InAnnotation}
	{
		std::cerr << "  ... Annotator()" << std::endl;
	}

	AnnotatedString operator()(std::string InString) const
	{
		std::cerr << "  ... in Annotator::operator()" << std::endl;
		return {InString, Annotation};
	}

	std::string Annotation;
};

std::string ToString(const AnnotatedString& InString)
{
	std::cerr << "  ... in ToString(AnnotatedString)" << std::endl;
	return InString.String + " " + InString.Annotation;
}

std::string AnnotationToString(const AnnotatedString& InString)
{
	std::cerr << "  ... in AnnotationToString()" << std::endl;
	return InString.String + " " + InString.Annotation;
}

int main()
{
	Annotator A{"(emphasized)"};
	std::function<AnnotatedString(std::string)> Fun{[A](auto X) { return A(X); }};

	Pipeline<int, std::string> ClassPipeline{Source(AddOne) | ToString | Emphasize | A | ToString};
	std::cout << ClassPipeline(1) << std::endl;

	Pipeline<int, std::string> ClassPipeline2{Source(AddOne) | ToString | Emphasize | Fun | ToString};
	std::cout << (ClassPipeline2 | Emphasize)(2) << std::endl;

	auto FunPipeline{Seq(AddOne, IntToString, Emphasize, Emphasize, Length, IntToString, Emphasize, A, AnnotationToString)};
	std::cout << FunPipeline(4) << std::endl;

	auto FunPipeline2{
		Seq(AddOne, IntToString, Emphasize, Emphasize, Length, IntToString, Emphasize, Fun, AnnotationToString)};
	std::cout << Seq(FunPipeline2, Emphasize)(4) << std::endl;
	return 0;
}
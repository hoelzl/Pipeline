#pragma once

#include <functional>
#include <type_traits>

template <typename TArg, typename TResult>
class Pipeline
{
public:
	Pipeline(std::function<TResult(TArg)> InFunction) : Function{InFunction}
	{
	}

	TResult operator()(TArg InArg) const
	{
		return Function(InArg);
	}

private:
	std::function<TResult(TArg)> Function;
};

template <typename TArg, typename TResult>
Pipeline<TArg, TResult> Source(std::function<TResult(TArg)> InFunction)
{
	return Pipeline<TArg, TResult>{InFunction};
}

template <typename TArg, typename TResult>
Pipeline<TArg, TResult> Source(TResult (*InFunction)(TArg))
{
	return Pipeline<TArg, TResult>{InFunction};
}

template <typename TArg, typename TResult, typename TResult2>
Pipeline<TArg, TResult2> operator|(const Pipeline<TArg, TResult>& InPipeline1, Pipeline<TResult, TResult2> InPipeline2)
{
	return Pipeline<TArg, TResult2>{[InPipeline1, InPipeline2](TArg InArg) { return (InPipeline2(InPipeline1(InArg))); }};
}

template <typename TArg, typename TResult, typename TResult2>
Pipeline<TArg, TResult2> operator|(const Pipeline<TArg, TResult>& InPipeline, std::function<TResult2(TResult)> InFunction)
{
	return Pipeline<TArg, TResult2>{[InPipeline, InFunction](TArg InArg) { return (InFunction(InPipeline(InArg))); }};
}

template <typename TArg, typename TResult, typename TResult2>
Pipeline<TArg, TResult2> operator|(const Pipeline<TArg, TResult>& InPipeline, TResult2 (*InFunction)(TResult))
{
	return Pipeline<TArg, TResult2>{[InPipeline, InFunction](TArg InArg) { return (InFunction(InPipeline(InArg))); }};
}

template <typename TArg, typename TResult, typename C>
auto operator|(const Pipeline<TArg, TResult>& InPipeline, C InObject) -> Pipeline<TArg, decltype(InObject(TResult{}))>
{
	auto Fun = [InPipeline, InObject](TArg InArg) { return std::invoke(InObject, InPipeline(InArg)); };
	return {std::move(Fun)};
}

template <typename TArg, typename TResult, typename TResult2>
Pipeline<TArg, TResult2> operator|(Pipeline<TArg, TResult> InPipeline, std::function<TResult2(const TResult&)> InFunction)
{
	return Pipeline<TArg, TResult2>{[InPipeline, InFunction](TArg InArg) { return (InFunction(InPipeline(InArg))); }};
}

template <typename TArg, typename TResult, typename TResult2>
Pipeline<TArg, TResult2> operator|(Pipeline<TArg, TResult> InPipeline, TResult2 (*InFunction)(const TResult&))
{
	return Pipeline<TArg, TResult2>{[InPipeline, InFunction](TArg InArg) { return (InFunction(InPipeline(InArg))); }};
}

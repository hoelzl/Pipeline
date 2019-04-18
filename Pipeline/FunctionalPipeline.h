#pragma once

#include <functional>

template <typename TArg, typename TResult, typename TResult2>
std::function<TResult2(TArg)> Seq(TResult (*InFunction1)(TArg), TResult2 (*InFunction2)(TResult))
{
	return std::function<TResult2(TArg)>{
		[InFunction1, InFunction2](TArg InArg) { return (InFunction2(InFunction1(InArg))); }};
}

template <typename TArg, typename TResult, typename TResult2>
std::function<TResult2(TArg)> Seq(TResult (*InFunction1)(TArg), std::function<TResult2(TResult)> InFunction2)
{
	return std::function<TResult2(TArg)>{
		[InFunction1, InFunction2](TArg InArg) { return (InFunction2(InFunction1(InArg))); }};
}

template <typename TArg, typename TResult, typename TResult2>
std::function<TResult2(TArg)> Seq(std::function<TResult(TArg)> InFunction1, TResult2 (*InFunction2)(TResult))
{
	return std::function<TResult2(TArg)>{
		[InFunction1, InFunction2](TArg InArg) { return (InFunction2(InFunction1(InArg))); }};
}

template <typename TArg, typename TResult, typename TResult2>
std::function<TResult2(TArg)> Seq(std::function<TResult(TArg)> InFunction1, std::function<TResult2(TResult)> InFunction2)
{
	return std::function<TResult2(TArg)>{
		[InFunction1, InFunction2](TArg InArg) { return (InFunction2(InFunction1(InArg))); }};
}

///

template <typename TArg, typename TResult, typename TResult2>
std::function<TResult2(TArg)> Seq(TResult (*InFunction1)(TArg), TResult2 (*InFunction2)(const TResult&))
{
	return std::function<TResult2(TArg)>{
		[InFunction1, InFunction2](TArg InArg) { return (InFunction2(InFunction1(InArg))); }};
}

template <typename TArg, typename TResult, typename TResult2>
std::function<TResult2(TArg)> Seq(TResult (*InFunction1)(TArg), std::function<TResult2(const TResult&)> InFunction2)
{
	return std::function<TResult2(TArg)>{
		[InFunction1, InFunction2](TArg InArg) { return (InFunction2(InFunction1(InArg))); }};
}

template <typename TArg, typename TResult, typename TResult2>
std::function<TResult2(TArg)> Seq(std::function<TResult(TArg)> InFunction1, TResult2 (*InFunction2)(const TResult&))
{
	return std::function<TResult2(TArg)>{
		[InFunction1, InFunction2](TArg InArg) { return (InFunction2(InFunction1(InArg))); }};
}

template <typename TArg, typename TResult, typename TResult2>
std::function<TResult2(TArg)> Seq(std::function<TResult(TArg)> InFunction1,
								  std::function<TResult2(const TResult&)> InFunction2)
{
	return std::function<TResult2(TArg)>{
		[InFunction1, InFunction2](TArg InArg) { return (InFunction2(InFunction1(InArg))); }};
}

///

template <typename TArg, typename TResult, typename C>
auto Seq(TResult (*InFunction)(TArg), C InObject) -> std::function<decltype(InObject(TResult{}))(TResult)>
{
	auto Fun = [InFunction, InObject](TArg InArg) {
		TResult IntermediateResult{InFunction(InArg)};
		decltype(InObject(TResult{})) InnerResult = std::invoke(InObject, IntermediateResult);
		return InnerResult;
	};
	return Fun;
}

template <typename TArg, typename TResult, typename C>
auto Seq(std::function<TResult(TArg)> InFunction, C InObject) -> std::function<decltype(InObject(TResult{}))(TResult)>
{
	auto Fun = [InFunction, InObject](TArg InArg) {
		TResult IntermediateResult{InFunction(InArg)};
		decltype(InObject(TResult{})) InnerResult = std::invoke(InObject, IntermediateResult);
		return InnerResult;
	};
	return Fun;
}

template <typename TArg, typename TResult, typename C>
auto Seq(C InObject, TResult (*InFunction)(TArg)) /* -> std::function<decltype(TResult(TArg{}))> */
{
	auto Fun = [InFunction, InObject](auto InArg) {
		auto IntermediateResult{std::invoke(InObject, InArg)};
		TResult InnerResult = InFunction(IntermediateResult);
		return InnerResult;
	};
	return Fun;
}

template <typename TArg, typename TResult, typename C>
auto Seq(C InObject, std::function<TResult(TArg)> InFunction) /* -> std::function<decltype(TResult(TArg{}))> */
{
	auto Fun = [InFunction, InObject](auto InArg) {
		auto IntermediateResult{std::invoke(InObject, InArg)};
		TResult InnerResult = std::invoke(InFunction, IntermediateResult);
		return InnerResult;
	};
	return Fun;
}

template <typename Tf, typename Tg, typename Th, typename... Ts>
auto Seq(Tf F, Tg G, Th H, Ts... Fs)
{
	return Seq(F, Seq(G, H, Fs...));
}

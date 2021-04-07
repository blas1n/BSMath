#pragma once

#include <random>

namespace BSMath
{
	namespace Detail
	{
		template <class Distributor>
		struct HasParam final
		{
		private:
			template <typename C>
			static std::true_type Test(typename C::param_type*);

			template <typename C>
			static std::false_type Test(...);

		public:
			constexpr static bool value = decltype(Test<Distributor>(0))::value;
		};
	}

	template <class T, class Engine, class Distributor, bool = Detail::HasParam<Distributor>::value>
	class Random;

	template <class T, class Engine, class Distributor>
	class Random<T, Engine, Distributor, true> final
	{
	public:
		using Seed = typename Engine::result_type;
		using Parameter = typename Distributor::param_type;

	public:
		Random() : engine(std::random_device{}()), distributor() {}
		Random(const Parameter& param) : engine(std::random_device{}()), distributor(param) {}

		T operator()() noexcept
		{
			return distributor(engine);
		}

		T operator()(const Parameter& param) noexcept
		{
			return distributor(engine, param);
		}

		void SetSeed(Seed seed)
		{
			engine.seed(seed);
		}

	private:
		Engine engine;
		Distributor distributor;
	};

	template <class T, class Engine, class Distributor>
	class Random<T, Engine, Distributor, false> final
	{
	public:
		using Seed = typename Engine::result_type;

	public:
		Random() : engine(std::random_device{}()), distributor() {}

		T operator()() noexcept
		{
			return distributor(engine);
		}

		void SetSeed(Seed seed)
		{
			engine.seed(seed);
		}

	private:
		Engine engine;
		Distributor distributor;
	};
	
	using UniformIntRandom = Random<int, std::mt19937, std::uniform_int_distribution<int>>;
	using UniformFloatRandom = Random<float, std::mt19937, std::uniform_real_distribution<float>>;
	using NormalFloatRandom = Random<float, std::mt19937, std::normal_distribution<float>>;
}

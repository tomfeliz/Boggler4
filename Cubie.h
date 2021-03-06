#pragma once

#include <vector>

namespace Boggler
{
	template<typename T>
	class Cubie
	{
		public:
			Cubie();
			Cubie(T val, int cubieNum);
			~Cubie() {}
			void SetNeighbors(const std::vector<Cubie<T>*>& neighbors);
			const std::vector<Cubie<T>*>& GetNeighbors() const;
			const T & GetValue() const;
			void SetValue(const T &value);
		int GetCubieNumber() const;
			void SetCubieNumber(const int value);
			bool operator==(const Cubie<T> &other) const;
			bool operator!=(const Cubie<T> &other) const;

		private:
			std::vector<Cubie<T>*> _neighbors;
			T _value;
			int _cubieNumber;
	};
}


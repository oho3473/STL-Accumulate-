#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<list>

using int_vector = std::vector<int>;
using history = std::map<int, std::shared_ptr< int_vector>>;

//accumulate

int CanAccumulate(int sum, const int_vector& numbers, std::map<int,bool>& result)
{
	if (result.count(sum) == 1)
	{
		return result[sum];
	}

	if (sum == 0)
	{
		return true;
	}
	
	if(sum < 0)
	{
		return false;
	}

	int remain;

	for (auto e : numbers)
	{
		remain = sum - e;
		if (CanAccumulate(remain, numbers,result) == true)
		{
			return true;
		}
	}

	result[sum] = false;
	return false;
}

std::shared_ptr<int_vector> HowAccumulate(int sum, const int_vector& numbers,std::shared_ptr<history> h)
{
	std::vector<int> newvector{};

	if (h->count(sum) == 1)
	{
		return (*h)[sum];
	}

	if (sum == 0)
	{
		return std::make_shared< int_vector>();
	}

	if (sum < 0)
	{
		return nullptr;
	}

	int remain;

	for (auto e : numbers)
	{
		remain = sum - e;
		auto ret = HowAccumulate(remain, numbers, h);

		if (ret != nullptr)
		{
			ret->push_back(e);
			(*h)[sum] = ret;
			return (*h)[sum];
		}
	}

	(*h)[sum] = nullptr;
	return nullptr;
}

std::shared_ptr<int_vector> OptimizeAccumulate(int sum, const int_vector& numbers, std::shared_ptr<history> h)
{
	std::vector<int> newvector{};

	if (h->count(sum) == 1)
	{
		return (*h)[sum];
	}

	if (sum == 0)
	{
		return std::make_shared< int_vector>();
	}

	if (sum < 0)
	{
		return nullptr;
	}

	int remain;
	std::shared_ptr<int_vector> optimized = nullptr;

	for (auto e : numbers)
	{
		remain = sum - e;
		auto ret = OptimizeAccumulate(remain, numbers, h);

		if (ret != nullptr)
		{
			std::shared_ptr<int_vector> v = std::make_shared<int_vector>();
			v->resize(ret->size());
			std::copy(ret->begin(), ret->end(), v->begin());
			v->push_back(e);

			if (optimized == nullptr || v->size() < optimized->size())
			{
				optimized = v;
			}

			
		}
	}

	(*h)[sum] = optimized;
	return (*h)[sum];
}

void Print(int_vector* v)
{
	std::cout << "{ ";
	
	if (v != nullptr)
	{
		for (auto e : *v)
		{
			std::cout << e << " ";
		}
	}

	std::cout << "}" << std::endl;
}

//accumulate -vector-

bool CanAccumulate(int sum, const std::vector<int>& numbers)
{
	std::vector<bool> array(sum + 1);

	array[0] = true;

	for (int i = 0; i <= sum; ++i)
	{
		if (array[i] == true)
		{
			for ( auto e : numbers)
			{
				if (i + e <= sum)
				{
					array[i + e] = true;
				}
			}
		}if (array[i] == true)
		{
			for ( auto e : numbers)
			{
				if (i + e <= sum)
				{
					array[i + e] = true;
				}
			}
		}
	}

	return array[sum];
}

std::shared_ptr<std::vector<int>> HowAccumulate(int sum, const std::vector<int>& numbers)
{
	std::vector<std::shared_ptr<std::vector<int>>> table(sum + 1,nullptr);

	table[0] = std::make_shared<std::vector<int>>(0);

	for (int i = 0; i <= sum; ++i)
	{
		if (table[i] != nullptr)
		{
			for (auto e : numbers)
			{
				if (i + e <= sum)
				{
					table[i + e] = std::make_shared<std::vector<int>>(*table[i]);

					table[i + e]->push_back(e);
				}
			}
		}
	}
	
	return table[sum];
}

std::shared_ptr<std::vector<int>> OptimizeAccumulate(int sum, const std::vector<int>& numbers)
{
	std::vector<std::shared_ptr<std::vector<int>>> table(sum + 1, nullptr);

	table[0] = std::make_shared<std::vector<int>>(0);

	for (int i = 0; i <= sum; ++i)
	{
		if (table[i] != nullptr)
		{
			for (auto e : numbers)
			{
				if (i + e <= sum)
				{
					if (table[i + e] == nullptr || (table[i+e]->size()-1 > table[i]->size()))
					{
						table[i + e] = std::make_shared<std::vector<int>>(*table[i]);

						table[i + e]->push_back(e);
					}
				}
			}
		}
	}

	return table[sum];
}

int main()
{
	std::cout << std::boolalpha << CanAccumulate(8, {2,3,5}) << std::endl;
	
	auto ret = HowAccumulate(8, { 2,3,5 });
	std::cout << "{";
	if (ret != nullptr)
	{
		for (auto e : *ret)
		{
			std::cout << e << ", ";
		}
	}
	std::cout << "}" << std::endl;
	
	auto ret1 = OptimizeAccumulate(8, { 2,3,5 });
	std::cout << "{";
	if (ret != nullptr)
	{
		for (auto e : *ret1)
		{
			std::cout << e << ", ";
		}
	}
	std::cout << "}" << std::endl;
	
	
	std::map<int, bool> h;
	
	Print(OptimizeAccumulate(8, { 1,2,5,3,4,8 }, std::make_shared<history>()).get());
	
	return 0;
}

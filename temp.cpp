#include <iostream>

template <typename T>
void
print(T t)
{
	std::cout << t << std::endl;
}

template <typename T> struct A {
	void
	print(T t)
	{
		::print<T>(t);
	};
};

template <typename T> union var_bT {
	T var;
	unsigned char byte[sizeof(T)];
};

int
main()
{
	var_bT<float[4]> x;

	for (size_t i = 0; i < 4; ++i) {
		x.var[i] = i;
		std::cout << "0x" << std::hex << +x.byte[i] << " ";
	}

	std::cout << std::endl;
	x.var[0] = 1;
	x.var[1] = 0;
	x.var[2] = 1e6;
	x.var[3] = -6e-1;

	for (size_t i = 0; i < sizeof(float[4]); ++i) {
		std::cout << "0x" << std::hex << +x.byte[i] << " ";
	}
}

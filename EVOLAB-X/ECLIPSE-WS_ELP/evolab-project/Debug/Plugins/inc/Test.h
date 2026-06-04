#ifndef __ACLASS_H__
#define __ACLASS_H__

template < typename T >
class A {
public:

	A();

	T getB();
	void setB(T b);

private:
	T _b;
};

template < typename T > A<T>::A() { _b = (T)0; }

template < typename T > T A<T>::getB() { return _b; }

template < typename T > void A<T>::setB(T b) { _b = b; }

#endif

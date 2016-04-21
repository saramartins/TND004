#include "set.h"

template<typename T>
Set::Set() {
}

template<typename T>
Set::Set(const Set &s)
{

}

template<typename T>
~Set::Set(){

}

template<typename T>
Set::Set(T value){
    return false;
}

template<typename T> // Move constructor (noexcept to make sure it uses move instead of copy)
Set::Set(Set &&R) noexcept{
    return false;
}


template<typename T>
Set::Set(T a[], int n){
    return false;
}

template<typename T>// Assignment operator
const Set&operator=(const Set &R){
    return false;
}


template<typename T>// Move Assignment operator
const Set&Set::operator=(Set &&R) noexcept{
    return false;
}



template<typename T>// Function to test if Set is empty
bool Set::_empty() const{

    return false;
}

template<typename T>// How many elements in the Set
int Set::cardinality() const{

    return false;
}

template<typename T>// Test if v belongs to the Set
bool Set::is_member(T value) const{

    return false;
}

template<typename T>// set the Set to empty
void Set::make_empty(){
}

// Overloaded operators
template<typename T>
Set Set::operator+=(const Set &R) const{
    return false;
}

template<typename T>
Set Set::operator*=(const Set &R) const{
    return false;
}

template<typename T>
Set Set::operator-=(const Set &R) const{
    return false;
}

template<typename T>
Set Set::operator+(const Set &R) const{
    return false;
}

template<typename T>
Set Set::operator*(const Set &R) const{
    return false;
}

template<typename T>
Set Set::operator-(const Set &R) const{
    return false;
}

    // Comparison operators
template<typename T>
bool Set::operator<=(const Set &R){
    return false;
}

template<typename T>
bool Set::operator<(const Set &R) const{
    return false;
}

template<typename T>
bool Set::operator==(const Set &R){
    return false;
}

template<typename T> //returns true if R(S) has an element that does not belong to S(R)
bool Set::operator!=(const Set &R){
    return false;
}


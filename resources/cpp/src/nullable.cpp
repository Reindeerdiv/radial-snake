#include "nullable.h"

template <typename T>
Nullable<T>::Nullable(T value): _value(value) {
  _initialized = true;
}

template <typename T>
Nullable<T>::Nullable() {

}

template <typename T>
T Nullable::getValue() const {
  return _value;
}

template <typename T>
void Nullable::setValue(T value) const {
  _value = value;
  _initialized = true;
}

template <typename T>
void Nullable::resetValue() {
  _initialized = false;
}

template <typename T>
bool Nullable<T>::hasValue() const {
  return &_value != 0;
}

template <typename T>
bool Nullable<T>::isNull() const {
  return &_value == 0;
}
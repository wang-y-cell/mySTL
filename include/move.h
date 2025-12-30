//简单实现move语义


#ifndef MYSTL_MOVE_H
#define MYSTL_MOVE_H

#include "type_traits.h"

namespace msl {

//处理左值
template<typename _Tp>
constexpr _Tp&&
forward(typename msl::remove_reference<_Tp>::type& __t) noexcept{
  return static_cast<_Tp&&>(__t); 
}

//处理右值
template<typename _Tp>
constexpr _Tp&&
forward(typename msl::remove_reference<_Tp>::type&& __t) noexcept{
  return static_cast<_Tp&&>(__t);
}


template<typename T>
constexpr typename msl::remove_reference<T>::type&& 
move(T&& t) noexcept {
    return static_cast<typename msl::remove_reference<T>::type&&>(t);
}

}




#endif
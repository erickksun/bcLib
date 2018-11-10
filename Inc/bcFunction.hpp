/********************************************************
* Copyright：The key code of this file comes from the Internet and I modified a little.
* Function： realized the std::function, And you can use it in the compiler before C++11
* Note：Please read the code,and uses it.Just like the DEMO.
* Author：sun
* Date：2018.11.09
*******************************************************/

/*
// The number of parameters supported by default is up to 21,
// if it cannot meet your requirements, you can modify it yourself at the end of this document.
// DEMO
#include "bcFunction.hpp"
#include <iostream>
struct Test
{
    int foo(int,char)
    {
        std::cout << "[" << __FUNCTION__<< "," << __LINE__ << "]"<< std::endl;
        return __LINE__;
    }
} test;

int foo(int,char)
{
    std::cout << "[" << __FUNCTION__<< "," << __LINE__ << "]"<< std::endl;
    return __LINE__;
}

int main()
{
    bc::Function<int (int,char)> foo1;
    if (foo1.IsValide())
        foo1(0,'0');
    foo1 = bc::Function<int (int,char)>::Bind(&foo);

    bc::Function<int (int,char)> foo2;
    foo2 = bc::Function<int (int,char)>::Bind(&test, &Test::foo);
    std::cout << foo1(0,'0') << std::endl;
    std::cout << foo2(0,'0') << std::endl;

    return 0;
}
*/

#ifndef _BC_FUNCTION_HPP_
#define _BC_FUNCTION_HPP_

namespace bc
{

template <typename R>
class FunctionBase0
{
public:
    virtual R Invoke() = 0;
    virtual ~FunctionBase0() {}
};


template <typename R, typename T>
class Function0 : public FunctionBase0<R>
{
public:
    R Invoke()
    {
        return m_Fun();
    }
public:
    Function0(const T &fun)
        : m_Fun(fun)
    {

    }
private:
    T m_Fun;
};


template <typename R, typename T>
class MemberFunction0 : public FunctionBase0<R>
{
public:
    R Invoke()
    {
        return (m_pObj->*m_pMemFun)();
    }
public:
    MemberFunction0(T *pObj, R(T::*pMemFun)())
        : m_pObj(pObj), m_pMemFun(pMemFun)
    {

    }
private:
    T *m_pObj;
    R(T::*m_pMemFun)();
};

template <typename Signature>
class Function;
template <typename R>
class Function<R()>
{
public:
    template <typename T>
    Function(const T &fun)
        : m_pFunBase(new Function0<R, T>(fun))
    {

    }

    template <typename T>
    Function(T *pObj, R(T::*pMemFun)())
        : m_pFunBase(new MemberFunction0<R, T>(pObj, pMemFun))
    {

    }

    Function(): m_pFunBase(0)
    {

    }

    template <typename T>
    void operator = (const T &fun)
    {
        m_pFunBase = (FunctionBase0<R>*)(fun);
    }

    bool IsValide ()
    {
        return m_pFunBase != (FunctionBase0<R>*)(0);
    }

    template <typename T>
    static FunctionBase0<R>* Bind(T* pObj, R(T::*pMemFun)())
    {
        return new MemberFunction0<R, T>(pObj, pMemFun);
    };

    template <typename T>
    static FunctionBase0<R>* Bind(const T &fun)
    {
        return new Function0<R, T>(fun);
    };

    ~Function()
    {
		if (m_pFunBase != 0)
            delete m_pFunBase;
    }

    R operator ()()
    {
        return m_pFunBase->Invoke();
    }

private:
    FunctionBase0<R>* m_pFunBase;
};



template <typename Signature>
struct FunctionTraits
{
    typedef Signature ParamType;
};

template <typename RetType>
struct FunctionTraits<RetType()>
{
    typedef RetType(&ParamType)();
};
template <typename Signature>
class Function;


#define _BC_FUNCTION_MULTI_PARAM(n)                                        \
template <typename R, _BC_TYPENAME_DECLARE(n)>                             \
class FunctionBase_##n                                                     \
{                                                                          \
public:                                                                    \
    virtual R Invoke(_BC_TYPENAME_LIST(n)) = 0;                            \
    virtual ~FunctionBase_##n() {}                                         \
};                                                                         \
                                                                           \
                                                                           \
template <typename R, _BC_TYPENAME_DECLARE(n), typename T>                 \
class Function_##n : public FunctionBase_##n<R, _BC_TYPENAME_LIST(n)>      \
{                                                                          \
public:                                                                    \
    R Invoke(_BC_TYPENAME_VARIABLE(n))                                     \
    {                                                                      \
        return m_Fun(_BC_VARIABLE_LIST(n));                                \
    }                                                                      \
                                                                           \
public:                                                                    \
    Function_##n(const T &fun)                                             \
        : m_Fun(fun)                                                       \
    {                                                                      \
                                                                           \
    }                                                                      \
                                                                           \
private:                                                                   \
    T m_Fun;                                                               \
};                                                                         \
                                                                           \
template <typename R, typename P, typename T, _BC_TYPENAME_DECLARE(n)>     \
class MemberFunction_##n : public FunctionBase_##n<R, _BC_TYPENAME_LIST(n)>\
{                                                                          \
public:                                                                    \
    R Invoke(_BC_TYPENAME_VARIABLE(n))                                     \
    {                                                                      \
        return (m_pObj->*m_pMemFun)(_BC_VARIABLE_LIST(n));                 \
    }                                                                      \
                                                                           \
public:                                                                    \
    MemberFunction_##n(P pObj, R(T::*pMemFun)(_BC_TYPENAME_LIST(n)))       \
        : m_pObj(pObj), m_pMemFun(pMemFun)                                 \
    {                                                                      \
                                                                           \
    }                                                                      \
                                                                           \
private:                                                                   \
    P m_pObj;                                                              \
    R(T::*m_pMemFun)(_BC_TYPENAME_LIST(n));                                \
};                                                                         \
                                                                           \
template <typename RetType, _BC_TYPENAME_DECLARE(n)>                       \
struct FunctionTraits<RetType(_BC_TYPENAME_LIST(n))>                       \
{                                                                          \
    typedef RetType(&ParamType)(_BC_TYPENAME_LIST(n));                     \
};                                                                         \
                                                                           \
template <typename R, _BC_TYPENAME_DECLARE(n)>                             \
class Function<R(_BC_TYPENAME_LIST(n))>                                    \
{                                                                          \
public:                                                                    \
    template <typename T>                                                  \
    Function(const T &fun)                                                 \
    : m_pFunBase(new Function_##n<R, _BC_TYPENAME_LIST(n), typename FunctionTraits<T>::ParamType>(fun))  \
    {                                                                      \
                                                                           \
    }                                                                      \
                                                                           \
    template <typename P, typename T>                                      \
    Function(P pObj, R(T::*pMemFun)(_BC_TYPENAME_LIST(n)))                 \
        : m_pFunBase(new MemberFunction_##n<R, P, T, _BC_TYPENAME_LIST(n)>(pObj, pMemFun))               \
    {                                                                      \
                                                                           \
    }                                                                      \
                                                                           \
    Function(): m_pFunBase(0)                                              \
    {                                                                      \
                                                                           \
    }                                                                      \
                                                                           \
    bool IsValide ()                                                       \
    {                                                                      \
        return m_pFunBase != (FunctionBase_##n<R, _BC_TYPENAME_LIST(n)>*)(0);                            \
    }                                                                      \
                                                                           \
    void operator = (FunctionBase_##n<R, _BC_TYPENAME_LIST(n)>* fun)       \
    {                                                                      \
        m_pFunBase = fun;                                                  \
    }                                                                      \
                                                                           \
    template <typename P, typename T>                                      \
    static FunctionBase_##n<R, _BC_TYPENAME_LIST(n)>* Bind(P pObj, R(T::*pMemFun)(_BC_TYPENAME_LIST(n)))   \
    {                                                                                                      \
        return new MemberFunction_##n<R, P, T, _BC_TYPENAME_LIST(n)>(pObj, pMemFun);                       \
    };                                                                                                     \
                                                                                                           \
    template <typename T>                                                                                  \
    static FunctionBase_##n<R, _BC_TYPENAME_LIST(n)>* Bind(const T &fun)                                   \
    {                                                                                                      \
        return new Function_##n<R, _BC_TYPENAME_LIST(n), typename FunctionTraits<T>::ParamType>(fun);      \
    };                                                                     \
                                                                           \
                                                                           \
    ~Function()                                                            \
    {                                                                      \
	    if (m_pFunBase != 0)                                               \
            delete m_pFunBase;                                             \
    }                                                                      \
                                                                           \
    R operator ()(_BC_TYPENAME_VARIABLE(n))                                \
    {                                                                      \
        return m_pFunBase->Invoke(_BC_VARIABLE_LIST(n));                   \
    }                                                                      \
                                                                           \
private:                                                                   \
    FunctionBase_##n<R, _BC_TYPENAME_LIST(n)>* m_pFunBase;                 \
};

/*
TYPENAME_DECLARE(n)  is typename T0, typename T1, …, typename Tn
TYPENAME_LIST(n)     is T0, T1, …, Tn
TYPENAME_VARIABLE(n) is T0 v0, T1 v1, …, Tn vn
VARIABLE_LIST(n)     is v0, v1, …, vn
*/
#define _BC_REP_0(C, n) C(0)
#define _BC_REP_1(C, n) _BC_REP_0(C, n) , C(1)
#define _BC_REP_2(C, n) _BC_REP_1(C, n) , C(2)
#define _BC_REP_3(C, n) _BC_REP_2(C, n) , C(3)
#define _BC_REP_4(C, n) _BC_REP_3(C, n) , C(4)
#define _BC_REP_5(C, n) _BC_REP_4(C, n) , C(5)
#define _BC_REP_6(C, n) _BC_REP_5(C, n) , C(6)
#define _BC_REP_7(C, n) _BC_REP_6(C, n) , C(7)
#define _BC_REP_8(C, n) _BC_REP_7(C, n) , C(8)
#define _BC_REP_9(C, n) _BC_REP_8(C, n) , C(9)
#define _BC_REP_10(C, n) _BC_REP_9(C, n) , C(10)
#define _BC_REP_11(C, n) _BC_REP_10(C, n) , C(11)
#define _BC_REP_12(C, n) _BC_REP_11(C, n) , C(12)
#define _BC_REP_13(C, n) _BC_REP_12(C, n) , C(13)
#define _BC_REP_14(C, n) _BC_REP_13(C, n) , C(14)
#define _BC_REP_15(C, n) _BC_REP_14(C, n) , C(15)
#define _BC_REP_16(C, n) _BC_REP_15(C, n) , C(16)
#define _BC_REP_17(C, n) _BC_REP_16(C, n) , C(17)
#define _BC_REP_18(C, n) _BC_REP_17(C, n) , C(18)
#define _BC_REP_19(C, n) _BC_REP_18(C, n) , C(19)
#define _BC_REP_20(C, n) _BC_REP_19(C, n) , C(20)
#define _BC_REP_21(C, n) _BC_REP_20(C, n) , C(21)
#define _BC_REP_22(C, n) _BC_REP_21(C, n) , C(22)
#define _BC_REP_23(C, n) _BC_REP_22(C, n) , C(23)
#define _BC_REP_24(C, n) _BC_REP_23(C, n) , C(24)
#define _BC_REP_25(C, n) _BC_REP_24(C, n) , C(25)
#define _BC_REP_26(C, n) _BC_REP_25(C, n) , C(26)
#define _BC_REP_27(C, n) _BC_REP_26(C, n) , C(27)
#define _BC_REP_28(C, n) _BC_REP_27(C, n) , C(28)
#define _BC_REP_29(C, n) _BC_REP_28(C, n) , C(29)
#define _BC_REP_30(C, n) _BC_REP_29(C, n) , C(30)
#define _BC_REP(C, n)   _BC_REP_##n(C, n)

#define _BC_REP2_0(C, n) C(0)
#define _BC_REP2_1(C, n) _BC_REP2_0(C, n)  C(1)
#define _BC_REP2_2(C, n) _BC_REP2_1(C, n)  C(2)
#define _BC_REP2_3(C, n) _BC_REP2_2(C, n)  C(3)
#define _BC_REP2_4(C, n) _BC_REP2_3(C, n)  C(4)
#define _BC_REP2_5(C, n) _BC_REP2_4(C, n)  C(5)
#define _BC_REP2_6(C, n) _BC_REP2_5(C, n)  C(6)
#define _BC_REP2_7(C, n) _BC_REP2_6(C, n)  C(7)
#define _BC_REP2_8(C, n) _BC_REP2_7(C, n)  C(8)
#define _BC_REP2_9(C, n) _BC_REP2_8(C, n)  C(9)
#define _BC_REP2_10(C, n) _BC_REP2_9(C, n)  C(10)
#define _BC_REP2_11(C, n) _BC_REP2_10(C, n)  C(11)
#define _BC_REP2_12(C, n) _BC_REP2_11(C, n)  C(12)
#define _BC_REP2_13(C, n) _BC_REP2_12(C, n)  C(13)
#define _BC_REP2_14(C, n) _BC_REP2_13(C, n)  C(14)
#define _BC_REP2_15(C, n) _BC_REP2_14(C, n)  C(15)
#define _BC_REP2_16(C, n) _BC_REP2_15(C, n)  C(16)
#define _BC_REP2_17(C, n) _BC_REP2_16(C, n)  C(17)
#define _BC_REP2_18(C, n) _BC_REP2_17(C, n)  C(18)
#define _BC_REP2_19(C, n) _BC_REP2_18(C, n)  C(19)
#define _BC_REP2_20(C, n) _BC_REP2_19(C, n)  C(20)
#define _BC_REP2_21(C, n) _BC_REP2_20(C, n)  C(21)
#define _BC_REP2_22(C, n) _BC_REP2_21(C, n)  C(22)
#define _BC_REP2_23(C, n) _BC_REP2_22(C, n)  C(23)
#define _BC_REP2_24(C, n) _BC_REP2_23(C, n)  C(24)
#define _BC_REP2_25(C, n) _BC_REP2_24(C, n)  C(25)
#define _BC_REP2_26(C, n) _BC_REP2_25(C, n)  C(26)
#define _BC_REP2_27(C, n) _BC_REP2_26(C, n)  C(27)
#define _BC_REP2_28(C, n) _BC_REP2_27(C, n)  C(28)
#define _BC_REP2_29(C, n) _BC_REP2_28(C, n)  C(29)
#define _BC_REP2_30(C, n) _BC_REP2_29(C, n)  C(30)
#define _BC_REP2(C, n)    _BC_REP2_##n(C, n)

#define _BC_TYPENAME_DECLARE_PATTERN(n)     typename T##n
#define _BC_TYPENAME_DECLARE(n)             _BC_REP(_BC_TYPENAME_DECLARE_PATTERN, n)

#define _BC_TYPENAME_LIST_PATTERN(n)        T##n
#define _BC_TYPENAME_LIST(n)                _BC_REP(_BC_TYPENAME_LIST_PATTERN, n)

#define _BC_TYPENAME_VARIABLE_PATTERN(n)    T##n v##n
#define _BC_TYPENAME_VARIABLE(n)            _BC_REP(_BC_TYPENAME_VARIABLE_PATTERN, n)

#define _BC_VARIABLE_LIST_PATTERN(n)        v##n
#define _BC_VARIABLE_LIST(n)                _BC_REP(_BC_VARIABLE_LIST_PATTERN, n)

#define _BC_FUNCTION_IMPLEMENT(n)  _BC_REP2(_BC_FUNCTION_MULTI_PARAM, n)

/*UP TO 21 PARAMETERS*/
_BC_FUNCTION_IMPLEMENT(20)


} // namespace bc


#endif // _BC_FUNCTION_HPP_

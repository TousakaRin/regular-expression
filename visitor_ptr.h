#ifndef _VISITOR_PTR_H_
#define _VISITOR_PTR_H_

#include <cstddef>
#include <type_traits>
#include "objectPool.h"

namespace rgx{


template<typename elemType>
class visitor_ptr {
    template<typename Ptr>
	using _Convertible = typename std::enable_if<std::is_convertible<Ptr, elemType*>::value>::type;

    //template<typename Ptr>
    //using _CanConvertToPool = typename std::enable_if<std::is_convertible<elemType*, Ptr>::value>::type;

    template<typename T>
    friend class visitor_ptr;

    template<typename T>
    friend class _objectPool;

public:

    visitor_ptr(const visitor_ptr& v_ptr) : _rawPtr(v_ptr._rawPtr) {}

    template<typename T, typename = _Convertible<T*>>
	visitor_ptr(const visitor_ptr<T>& v_ptr) : 
        _rawPtr(static_cast<elemType*>(v_ptr._rawPtr))  { }

    visitor_ptr() : _rawPtr(nullptr) {}

    visitor_ptr(std::nullptr_t) : _rawPtr(nullptr) {}

    elemType *get()  { return this->operator->(); }

    elemType* operator->() {
        return _rawPtr;
    }

    const elemType* operator->() const {
        return _rawPtr; 
    }

    elemType& operator*() {
        return *_rawPtr; 
    }

    const elemType& operator*() const {
        return *_rawPtr;
    }

    visitor_ptr& operator=(const visitor_ptr& v_ptr) {
        _rawPtr = v_ptr._rawPtr;
        return *this;
    }

    bool operator==(const visitor_ptr& v_ptr) const {
        return _rawPtr == v_ptr._rawPtr;
    }
    
    bool operator!=(const visitor_ptr& v_ptr) const {
        return !this->operator==(v_ptr);
    }

    bool operator==(std::nullptr_t) const {
        return _rawPtr == nullptr;
    }

    bool operator!=(std::nullptr_t) const {
        return _rawPtr != nullptr;
    }

    operator bool() const {
        return _rawPtr != nullptr; 
    }

    bool operator <(const visitor_ptr &v_ptr) {
        return _rawPtr < v_ptr._rawPtr;
    }

    visitor_ptr& operator++() {
        ++_rawPtr;
        return *this;
    }
 
    visitor_ptr operator++(int) {
        auto copyed = *this;
        ++this;
        return copyed;
    }

private:
    visitor_ptr (elemType * ptr) : _rawPtr(ptr) {};
    elemType * _rawPtr;

};


}

#endif


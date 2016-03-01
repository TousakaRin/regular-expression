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

    template<typename Ptr>
    using _CanConvertToPool = typename std::enable_if<std::is_convertible<elemType*, Ptr>::value>::type;

    template<typename T>
    friend class visitor_ptr;

    template<typename T>
    friend class _objectPool;

public:

    visitor_ptr(const visitor_ptr& v_ptr) : _index(v_ptr._index), _objPool(v_ptr._objPool) {}

    template<typename T, typename = _Convertible<T*>>
	visitor_ptr(const visitor_ptr<T>& v_ptr) : 
        _index(v_ptr._index), 
        _objPool(v_ptr._objPool)  { }

    visitor_ptr() : _index(0), _objPool(nullptr) {}

    visitor_ptr(std::nullptr_t) : _index(0), _objPool(nullptr) {}

    unsigned int index() const { return _index; }

    elemType *get()  { return this->operator->(); }

    elemType* operator->() {
        return static_cast<elemType*>(static_cast<const _objectPool<elemType>*>(_objPool)->_pool[_index].get());
    }

    const elemType* operator->() const {
        return static_cast<elemType*>(static_cast<const _objectPool<elemType>*>(_objPool)->_pool[_index].get());
    }

    elemType& operator*() {
        return *(static_cast<const _objectPool<elemType>*>(_objPool)->_pool[_index]);
    }

    const elemType& operator*() const {
        return *(static_cast<const _objectPool<elemType>*>(_objPool)->_pool[_index]);
    }

    visitor_ptr& operator=(const visitor_ptr& v_ptr) {
        _index = v_ptr._index;
        _objPool = v_ptr._objPool;
        return *this;
    }

    bool operator==(const visitor_ptr& v_ptr) const {
        return _objPool == v_ptr._objPool && _index == v_ptr._index;
    }
    
    bool operator!=(const visitor_ptr& v_ptr) const {
        return !this->operator==(v_ptr);
    }

    bool operator==(std::nullptr_t) const {
        return _objPool == nullptr;
    }

    bool operator!=(std::nullptr_t) const {
        return _objPool != nullptr;
    }

    operator bool() const {
        return _objPool != nullptr; 
    }
//    template<typename T, typename = _Convertible<elemType*>>
//    operator visitor_ptr<T>() const {
//        return visitor_ptr<T>(_objPool, _index);    
//    }
    

private:
    template<typename poolElemType, typename = _CanConvertToPool<elemType*>>
    visitor_ptr(_objectPool<poolElemType> *pool, unsigned int index) : _index(index), _objPool(pool) {}
    unsigned int _index;
    const void *_objPool;

};


}

#endif


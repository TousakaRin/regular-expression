#ifndef _OBJHECT_POOL_
#define _OBJHECT_POOL_

#include <vector>
#include <memory>
#include <forward_list>

namespace rgx {

template<typename elemType>
class visitor_ptr;

template<typename elemType>
class _objectPool {
    template<typename Ptr>
	using _Convertible = typename std::enable_if<std::is_convertible<Ptr, elemType*>::value>::type;

    template<typename T>
    friend class visitor_ptr;

public:
    _objectPool() {}

    template<typename actualType, typename = _Convertible<actualType*>>
    _objectPool(const _objectPool<actualType>& objPool) : _pool(objPool._pool), _released(objPool._released) {} 

    template<typename actualType, typename... Args, typename = _Convertible<actualType*>>
    visitor_ptr<actualType> make_visitor(Args&&... args) {
        if (_released.empty()) {
            return visitor_ptr<actualType>(this, back_insert<actualType>(std::forward<Args>(args)...));
        } else {
            return visitor_ptr<actualType>(this, front_insert<actualType>(std::forward<Args>(args)...));
        }
    }

    template<typename actualType, typename = _Convertible<actualType*>>
    actualType* release(const visitor_ptr<actualType> &v_ptr) {
        if (v_ptr._objPool == this && v_ptr._index < _pool.size()) {
            _released.insert_after(_released.end(), v_ptr.index);
            return _pool[v_ptr._index].release();
        } 
        return nullptr;
    }

    template<typename actualType, typename = _Convertible<actualType*>>
    operator _objectPool<actualType>() {
        return _objectPool<actualType>(*this);     
    }

private:
    std::vector<std::unique_ptr<elemType>> _pool;
    std::forward_list<unsigned int> _released;
    template<typename actualType, typename... Args>
    unsigned int back_insert(Args&&... args) {
        _pool.push_back(std::unique_ptr<elemType>(new actualType(std::forward<Args>(args)...)));
        return _pool.size() - 1;
    }
    template<typename actualType, typename... Args>
    unsigned int front_insert(Args&&... args) {
        unsigned int index = _released.front();
        _released.erase_after(_released.before_begin());
        _pool[index].reset(new actualType(std::forward<Args>(args)...));
        return index;
    }
};


}
#endif

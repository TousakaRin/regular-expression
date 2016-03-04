#ifndef _OBJHECT_POOL_
#define _OBJHECT_POOL_

#include <vector>
#include <memory>
#include <forward_list>

namespace rgx {

template<typename elemType>
class visitor_ptr;

class _pattern;

template<typename elemType>
class _objectPool {
    template<typename Ptr>
	using _Convertible = typename std::enable_if<std::is_convertible<Ptr, elemType*>::value>::type;

    template<typename T>
    friend class visitor_ptr;

    friend class _pattern;

public:
    _objectPool() : _releasedCount(0) { before_end = _released.before_begin(); }

    template<typename actualType, typename... Args, typename = _Convertible<actualType*>>
    visitor_ptr<actualType> make_visitor(Args&&... args) {
        if (_released.empty()) {
            return visitor_ptr<actualType>(back_insert<actualType>(std::forward<Args>(args)...));
        } else {
            return visitor_ptr<actualType>(front_insert<actualType>(std::forward<Args>(args)...));
        }
    }

    elemType* release(unsigned int index) {
        if (index < _pool.size()) {
            ++_releasedCount;
            _released.insert_after(before_end, index);
            ++before_end;
            return _pool[index].release();
        } 
        return nullptr;
    }

    template<typename actualType, typename = _Convertible<actualType*>>
    operator _objectPool<actualType>() {
        return _objectPool<actualType>(*this);     
    }

    unsigned int capacity() {
        return _pool.size();
    }

    unsigned int size() {
        return capacity() - _releasedCount;
    }

private:
    std::vector<std::unique_ptr<elemType>> _pool;
    std::forward_list<unsigned int> _released;
    std::forward_list<unsigned int>::iterator before_end;
    unsigned int _releasedCount;

    template<typename actualType, typename... Args>
    actualType *back_insert(Args&&... args) {
        actualType * elem = new actualType(std::forward<Args>(args)...);
        _pool.push_back(std::unique_ptr<elemType>(elem));
        return elem;
    }

    template<typename actualType, typename... Args>
    actualType *front_insert(Args&&... args) {
        unsigned int index = _released.front();
        _released.erase_after(_released.before_begin());
        actualType* elem = new actualType(std::forward<Args>(args)...);
        _pool[index].reset(elem);
        return elem;
    }

    visitor_ptr<elemType> get_visitor(unsigned int index) {
        return visitor_ptr<elemType>(_pool[index].get());        
    }
};


}
#endif


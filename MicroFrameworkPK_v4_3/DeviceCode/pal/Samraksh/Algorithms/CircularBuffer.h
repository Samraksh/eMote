/**
 * @file   CircularBuffer.h
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Nov 6, 2014
 * @copyright (C) 2014 The Samraksh Company
 */

#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

#include <stddef.h>

template <typename T>
class CircularBufferT {
    T *m_buffer;
    size_t m_head;
    size_t m_tail;
    const size_t m_size;
    size_t m_length;

    size_t next(size_t current) {
        return (current + 1) % m_size;
    }

    size_t prev(size_t current) {
        return (current + m_size - 1) % m_size;
    }

public:
    CircularBufferT(const size_t size) : m_size(size), m_head(0), m_tail(0) {
        m_buffer = new T[size];
        m_length = 0;
    }

    virtual ~CircularBufferT() {
        delete [] m_buffer;
    }

    bool Push(const T &object) {
        size_t head = m_head;
        size_t nextHead = next(head);
        if (nextHead == m_tail) {
            return false;
        }
        m_buffer[head] = object;
        m_head = nextHead;
        ++m_length;
        return true;
    }

    bool Push(const T &object, T* storedObject) {
        bool ret = Push(object);
        if(ret) {
            size_t head = m_head;
            size_t prevHead = prev(head);
            *storedObject = &(m_buffer[prevHead]);
        }
        else {
            *storedObject = 0;
        }
        return ret;
    }

    bool Pop(T &object) {
        size_t tail = m_tail;
        if (tail == m_head) {
            return false;
        }
        object = m_buffer[tail];
        m_tail = next(tail);
        --m_length;
        return true;
    }

    size_t GetLength() {
        return m_length;
    }

    void Reset() {
        m_length = 0;
        m_head = 0;
        m_tail = 0;
    }
};


#endif /* _CIRCULARBUFFER_H_ */

/**
 * @file   Heap.h
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Nov 8, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief Customized heap for the running median.
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#include "Int.h"
#include "IntAttrib.h"
#include "Util.h"



template <typename IntElemT, typename T>
struct HeapDataT {
    IntElemT val; //!< used for heap comparisons.
    T** ptr; //!< pointer back to circular buffer.   //TODO: reference this type.
};



template <typename IntElemT, typename T, size_t N>
class HeapT {
public:
    //TODO: constructor allocates nextpow2(N)

    size_t m_size;
    size_t m_capacity;

    struct HeapDataT<IntElemT,T> *m_heap;

    struct HeapDataT<IntElemT,T> m_fake_data;
    struct HeapDataT<IntElemT,T>* m_ptr_fake_data;
    IntElemT m_oppositeValue;

    virtual ~HeapT() = 0;
    virtual struct HeapDataT<IntElemT,T>* Insert(IntElemT) = 0;

    /**
     * return root element.
     * A.K.A. "Peek" operation.
     */
    bool getRoot(struct HeapDataT<IntElemT,T>& s_data) {
        bool fRet = true;
        if(m_size > 0) {
            s_data = m_heap[0];
        }
        else {
            s_data = 0;
            fRet = false;
        }
        return fRet;
    }

    int getParent(int index) {
        int fRet = -1;
        if(index > 0) {
            int parent = (index-1)/2;
            if(parent < 0) {
                fRet = -1;
            }
            else {
                fRet = parent;
            }
        }
        return fRet;
    }

    int getLeft(int index) {
        int fRet = -1;
        int l = 2*index+1;
        if(l <= m_size-1) {
            fRet = l;
        }
        return fRet;
    }

    int getRight(int index) {
        int fRet = -1;
        int r = 2*index+2;
        if(r <= m_size) {
            fRet = r;
        }
        return fRet;
    }

    //IntElemT DeleteRoot(void) = 0;
    //bool RemoveLast(struct HeapDataT<IntElemT,T>& s_data) = 0;

    //void ResetHeapDataT(struct HeapDataT<IntElemT,T> & s_data) = 0;

    /**
     * checks if a pointer is inside the heap.
     */
    bool isMember(struct HeapDataT<IntElemT,T>* s_data) {
        bool fRet = false;
        if((s_data >= &(m_heap[0])) && (s_data < m_heap[m_size])) {
            fRet = true;
        }
        //TODO #ifdef DEBUG: if(fRet == false && ((s_data >= &(m_heap[0])) && (s_data < m_heap[m_capacity+1])) { ASSERT(false);} #endif
        return fRet;
    }
};

template <typename IntElemT, typename T, size_t N>
class MinHeapT : public HeapT<IntElemT,T,N>
{
public:
    MinHeapT();

    //TODO: API should accept function pointer for updating T on movement.
    /**
     * insert at free child of heap and work towards parent.
     */
    bool InsertMin(IntElemT &val, struct HeapDataT<IntElemT,IntElemT>* &s_data );

    bool getMin(IntElemT &val) {
        bool fRet = true;
        if(m_size > 0) {
            val = m_heap[0].val;
        }
        else {
            val = 0;
            fRet = false;
        }
        return fRet;
    }


    /**
     * @param ptr_heap node at which to start downward rule checking (toward children/leaves).
     */
    bool MinRuleCheckChildren(struct HeapDataT<IntElemT,T>* ptr_heap) {
        bool fRet = true;
        int index = 0;
        //DEBUG: ASSERT(isMember(ptr_heap));
        index = (ptr_heap - &(m_heap[0]))/&(m_heap[0]);
        fRet = MinRuleCheckChildren(index);
        return fRet;
    }

    /**
     * rule check, working away from the root node, examining children.
     * heap property allows
     * @param index place to begin rule checking in the heap.
     */
    bool MinRuleCheckChildren(int index) {
        bool fRet = true;
        if((index >= m_size-2) || (index < 0)) {
            //TODO: debug validate heap property
            fRet = true;
        }
        else {
            struct HeapDataT<IntElemT,T> temp ;
            int min, l, r;
            min = index;
            do {
                index = min;
                l = 2*index+1;
                r = 2*index+2;
                if((l < m_size) && (m_heap[min].val > m_heap[l].val)) {
                    min = l;
                }
                if((r < m_size) && (m_heap[min].val > m_heap[r].val)) {
                    min = r;
                }

                if(min != index) {
                    temp = m_heap[index];
                    m_heap[index] = m_heap[min];  //TODO: assert c99 for struct copy.
                    *(m_heap[index].ptr) = &(m_heap[index]);
                    m_heap[min] = temp;
                    *(m_heap[min].ptr) = &(m_heap[min]);
                }
            } while((min != index) && (min <= N/2-1));
        }
        return fRet;
    }

    /**
     * @param ptr_heap node at which to start upward rule checking (toward parent/root).
     */
    bool MinRuleCheckParent(struct HeapDataT<IntElemT,IntElemT>* ptr_heap) {
        bool fRet = true;
        int index = 0;
        //DEBUG: ASSERT(isMember(ptr_heap));
        index = (ptr_heap - &(m_heap[0]))/&(m_heap[0]);
        fRet = MinRuleCheckParent(index);
        return fRet;
    }

    bool MinRuleCheckParent(int index, struct HeapDataT<IntElemT,IntElemT>* &ptr_s_data ) {
        bool fRet = true;
        if((index > m_size) || (index <= 0)) {
            //TODO: debug validate heap property
            fRet = true;
        }
        else {
            struct HeapDataT<IntElemT,IntElemT> s_temp = m_heap[index];
            int parent = getParent(index);
            while((m_heap[parent].val > s_temp.val) && (parent >= 0)) {
                m_heap[index] = m_heap[parent];
                ((struct HeapDataT<IntElemT,IntElemT>*)*(m_heap[index].ptr)) = &(m_heap[index]);
                index = parent;
                parent = getParent(index);
            }
            m_heap[index] = s_temp;
            ptr_s_data = &(m_heap[index]);
        }
        return fRet;
    }

    bool DeleteMin(struct HeapDataT<IntElemT,T>& data)
    {
        bool fRet = true;
        if(m_size <= 0) {
            return false;
        }
        data = m_heap[0];
        m_heap[0] = m_heap[m_size-1];
        m_heap[m_size-1] = (HeapDataT<IntElemT,T>){IntAttribT<IntElemT>::Max(), 0}; //TODO: assert c99 mode for GCC.  //TODO: only include this line if debugging to help visual inspection.
        --m_size;
        MinRuleCheckChildren(0);
        return fRet;
    }

    bool RemoveLast(struct HeapDataT<IntElemT,T>& s_data)
    {
        bool fRet = true;
        if(m_size <= 0) {
           fRet = false;
        }
        else {
           int last = m_size;
           s_data = m_heap[last];
           m_heap[last] = (HeapDataT<IntElemT,T>){IntAttribT<IntElemT>::Max(), 0};
        }
        return fRet;
    }

    void Reset()
    {
        m_size = 0;
        for(int itr=0; itr < N+2; ++itr) {
            m_heap[itr] = m_oppositeValue;
        }
    }

};

template<typename IntElemT, typename T, size_t N>
HeapT<IntElemT,T,N>::MinHeapT() : HeapT<IntElemT,T,N>::HeapT()
{
   //TODO: assert N is a power of 2.
   m_heap = new struct HeapDataT<IntElemT,T>::HeapDataT[N + 2];
   m_size = 0;
   m_capacity = N;
   m_oppositeValue = IntAttribT<IntElemT>::Max();
   for(int itr=0; itr < N+2; ++itr) {
       m_heap[itr] = m_oppositeValue;
   }
}

template<typename IntElemT, typename T, size_t N>
bool MinHeapT<IntElemT,T,N>::InsertMin(IntElemT &val, struct HeapDataT<IntElemT,IntElemT>* &s_data ) {
    bool fRet = true;
    if(m_size >= m_capacity) {
        bool fRet = false;
        s_data = 0;
    }
    else {
        int index = m_size;
        ++m_size;
        m_heap[index].val = val;
        m_ptr_fake_data = &(m_heap[index]);     //init fake so MinHeapifyParent can work with new and existing nodes.
        m_heap[index].ptr = &m_ptr_fake_data;   //circular buffer is populated after InsertMin (ALT: create node in callee, pass node to InsertMin instead of value.)
        s_data = &(m_heap[index]);              //TODO: this line is not necessary.
        MinRuleCheckParent(index, s_data);
    }
    return fRet;
}


//////////////////////////////////////////////////////////////////////////////////////////////////

template <typename IntElemT, typename T, size_t N>
class MaxHeapT : public HeapT<IntElemT,T,N>
{
public:
    MaxHeapT(void) {
        //TODO: assert N is a power of 2.
        m_heap = new struct HeapDataT<IntElemT,T>[N + 2]();
        m_size = 0;
        m_capacity = N;
        m_oppositeValue = IntAttribT<IntElemT>::Min();
        for(int itr=0; itr < N+2; ++itr) {
            m_heap[itr] = m_oppositeValue;
        }
    }

    //TODO: API should accept function pointer for updating T on movement.
    bool InsertMax(IntElemT &val, struct HeapDataT<IntElemT,IntElemT>* &s_data ) {
        bool fRet = true;
        if(m_size >= m_capacity) {
            bool fRet = false;
            s_data = 0;
        }
        else {
            int index = m_size;
            ++m_size;
            int parent = getParent(index);
            while((m_heap[parent].val < val) && (parent >= 0)) {
                m_heap[index] = m_heap[parent];
                ((struct HeapDataT<IntElemT,IntElemT>*)*(m_heap[index].ptr)) = &(m_heap[index]);
                index = parent;
                parent = getParent(index);
            }
            m_heap[index].val = val;
            s_data = &(m_heap[index]);
        }
        return fRet;
    }

    bool getMax(IntElemT &val) {
        bool fRet = true;
        if(m_size > 0) {
            val = m_heap[0].val;
        }
        else {
            val = 0;
            fRet = false;
        }
        return fRet;
    }

    /**
     * convenience function
     * @param ptr_heap node at which to start downward rule checking (toward children/leaves).
     */
    bool MaxRuleCheckChildren(struct HeapDataT<IntElemT,T>* ptr_heap) {
        bool fRet = true;
        int index = 0;
        //DEBUG: ASSERT(isMember(ptr_heap));
        index = (ptr_heap - &(m_heap[0]))/&(m_heap[0]);
        fRet = MaxRuleCheckChildren(index);
        return fRet;
    }

    /**
     * rule check, working away from the root node, examining children.
     * heap property allows
     * @param index place to begin rule checking in the heap.
     */
    bool MaxRuleCheckChildren(int index) {
        bool fRet = true;
        if(index > m_size-1 || index < 0) {
            //TODO: debug validate heap property
            fRet = true;
        }
        else {
            struct HeapDataT<IntElemT,T> temp ;
            int max, l, r;
            max = index;
            do {
                index = max;
                l = 2*index+1;
                r = 2*index+2;
                if((l < m_size) && (m_heap[max].val > m_heap[l].val)) {
                    max = l;
                }
                if((r < m_size) && (m_heap[min].val > m_heap[r].val)) {
                    max = r;
                }

                if(max != index) {
                    temp = m_heap[index];
                    m_heap[index] = m_heap[max];  //TODO: assert c99 for struct copy.
                    *(m_heap[index].ptr) = &(m_heap[index]);
                    m_heap[max] = temp;
                    *(m_heap[max].ptr) = &(m_heap[max]);
                }
            } while((max != index) && (max <= N/2-1));
        }
        return fRet;

    }

    /**
     * @param ptr_heap node at which to start upward rule checking (toward parent/root).
     */
    bool MaxRuleCheckParent(struct HeapDataT<IntElemT,IntElemT>* ptr_heap) {
        bool fRet = true;
        int index = 0;
        //DEBUG: ASSERT(isMember(ptr_heap));
        index = (ptr_heap - &(m_heap[0]))/&(m_heap[0]);
        fRet = MaxRuleCheckParent(index);
        return fRet;
    }

    bool MaxRuleCheckParent(int index, struct HeapDataT<IntElemT,IntElemT>* &ptr_s_data ) {
        bool fRet = true;
        if((index > m_size) || (index <= 0)) {
            //TODO: debug validate heap property
            fRet = true;
        }
        else {
            struct HeapDataT<IntElemT,IntElemT> s_temp = m_heap[index];
            int parent = getParent(index);
            while((m_heap[parent].val > s_temp.val) && (parent >= 0)) {
                m_heap[index] = m_heap[parent];
                ((struct HeapDataT<IntElemT,IntElemT>*)*(m_heap[index].ptr)) = &(m_heap[index]);
                index = parent;
                parent = getParent(index);
            }
            m_heap[index] = s_temp;
            ptr_s_data = &(m_heap[index]);
        }
        return fRet;
    }

    bool DeleteMax(struct HeapDataT<IntElemT,T>& data)
    {
        bool fRet = true;
        if(m_size <= 0) {
            return false;
        }
        data = m_heap[0];
        m_heap[0] = m_heap[m_size-1];
        m_heap[m_size-1] = (HeapDataT<IntElemT,T>){IntAttribT<IntElemT>::Min(), 0}; //TODO: assert c99 mode for GCC.  //TODO: only include this line if debugging to help visual inspection.
        --m_size;
        MaxRuleCheckChildren(0);
        return fRet;
    }

    void Reset()
    {
        m_size = 0;
        for(int itr=0; itr < N+2; ++itr) {
            m_heap[itr] = m_oppositeValue;
        }
    }
};


#endif /* _HEAP_H_ */

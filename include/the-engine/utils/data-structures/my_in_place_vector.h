#pragma once
#include <cstddef>
#include <cassert>
#include <cstring>
#include <type_traits>
#include <new>
#include <span>
#include <utility>

namespace TheEngine
{
    template<typename T, std::size_t N>
    class MyInPlaceVector
    {
    private:

        alignas(T) std::byte m_data[sizeof(T) * N];
        size_t m_currentIndex = 0;



    private:

        T* getT(const std::size_t index)
        {
            assert(index <= m_currentIndex && "Tried to access invalid index");
            return reinterpret_cast<T*>(&m_data[index * sizeof(T)]);
        }

        const T* getT(const std::size_t index) const
        {
            assert(index <= m_currentIndex && "Tried to access invalid index");
            return reinterpret_cast<const T*>(&m_data[index * sizeof(T)]);
        }

    public:
        MyInPlaceVector() = default;

        ~MyInPlaceVector()
        {
            clear();
        }

      
        
        MyInPlaceVector(const MyInPlaceVector& other)
        {
            if constexpr (std::is_trivially_copyable_v<T>) 
            {
                std::memcpy(m_data, other.m_data, other.m_currentIndex * sizeof(T));
            }
            else 
            {
                for (size_t i = 0; i < other.m_currentIndex; ++i)
                {
                    new (getT(i)) T(other[i]);
                }
            }
            m_currentIndex = other.m_currentIndex;
        }
        
       
        MyInPlaceVector(MyInPlaceVector&& other) noexcept
        {
            for (size_t i = 0; i < other.m_currentIndex; ++i) 
            {
                new (getT(i)) T(std::move(other[i]));
            }
            m_currentIndex = other.m_currentIndex;
            other.m_currentIndex = 0; 

        }
        
   
        
        MyInPlaceVector& operator=(const MyInPlaceVector& other)
        {
            if (this == &other) return *this;
            clear();
            if constexpr (std::is_trivially_copyable_v<T>)
            {
                std::memcpy(m_data, other.m_data, other.m_currentIndex * sizeof(T));
            }
            else {
                for (size_t i = 0; i < other.m_currentIndex; ++i)
                {
                    new (getT(i)) T(other[i]);
                }
            }
            m_currentIndex = other.m_currentIndex;
            return *this;
        }
        

        
        MyInPlaceVector& operator=(MyInPlaceVector&& other) noexcept
        {
            if (this == &other) return *this;
            clear();
            for (size_t i = 0; i < other.m_currentIndex; ++i) 
            {
                new (getT(i)) T(std::move(other[i]));
            }
            m_currentIndex = other.m_currentIndex;
            other.m_currentIndex = 0;
            return *this;
        }
        


        void push_back(const T& data)
        {
            assert(m_currentIndex < N && "Vector overflow");
            new (getT(m_currentIndex)) T(data);
            m_currentIndex++;
        }

        void push_back(T&& data)
        {
            assert(m_currentIndex < N && "Vector overflow");
            new (getT(m_currentIndex)) T(std::move(data));
            m_currentIndex++;
        }

        void pop_back()
        {
            assert(m_currentIndex != 0 && "Vector underflow");
            --m_currentIndex;
            if constexpr (!std::is_trivially_destructible_v<T>) 
            {
                getT(m_currentIndex)->~T();
            }
        }

        void clear()
        {
            if constexpr (!std::is_trivially_destructible_v<T>) 
            {
                for (size_t i = 0; i < m_currentIndex; ++i) 
                {
                    getT(i)->~T();
                }
            }
            m_currentIndex = 0;
        }



        std::size_t size() const { return m_currentIndex; }
        std::size_t capacity() const { return N; }

        void reserve(const std::size_t newCapacity) 
        {
            assert(newCapacity <= N && "Reserve request exceeds fixed capacity");
        }



        T& operator[](const std::size_t index) { return *getT(index); }
        const T& operator[](const std::size_t index) const { return *getT(index); }


        T* data() { return reinterpret_cast<T*>(m_data); }
        const T* data() const { return reinterpret_cast<const T*>(m_data); }

      
        operator std::span<T>() { return std::span<T>(data(), m_currentIndex); }
        operator std::span<const T>() const { return std::span<const T>(data(), m_currentIndex); }
    };
}
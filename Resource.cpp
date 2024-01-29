#include <iostream>
#include "Resource.h"

Resource::Resource()
{
    std::cout << "Resource default constructor" << std::endl;
}

Resource::~Resource()
{
    std::cout << "Resource destructor" << std::endl;
    if (m_data != nullptr)
    {
        delete[] m_data;
    }
}

Resource::Resource(int size)
{
    std::cout << "Resource constructor" << std::endl;
    m_size = size;
    m_data = new int[m_size];
}

Resource::Resource(const Resource &r)
{
    std::cout << "Resource copy constructor" << std::endl;
    m_size = r.m_size;
    m_data = new int[m_size];
    std::copy(r.m_data, r.m_data + m_size, m_data);
}

Resource::Resource(Resource &&r)
{
    std::cout << "Resource move constructor" << std::endl;
    swap(*this, r);
}

Resource &Resource::operator=(Resource r)
{
    std::cout << "Resource assignment operator" << std::endl;
    swap(*this, r);
    return *this;
}

int &Resource::operator[](int index)
{
    return m_data[index];
}

void Resource::print()
{
    std::cout << "Resource data: ";
    for (int i = 0; i < m_size; ++i)
    {
        std::cout << m_data[i] << " ";
    }
    std::cout << std::endl;
}

void Resource::swap(Resource &a, Resource &b)
{
    std::swap(a.m_size, b.m_size);
    std::swap(a.m_data, b.m_data);
}

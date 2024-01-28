#include <bits/stdc++.h>

// Resource holding an array of integers
class Resource
{
public:
    Resource()
    {
        std::cout << "Resource default constructor" << std::endl;
    }

    ~Resource()
    {
        std::cout << "Resource destructor" << std::endl;
        if (m_data != nullptr)
        {
            delete[] m_data;
        }
    }

    Resource(int size)
    {
        std::cout << "Resource constructor" << std::endl;
        m_size = size;
        m_data = new int[m_size];
    }

    Resource(const Resource &r)
    {
        std::cout << "Resource copy constructor" << std::endl;
        m_size = r.m_size;
        m_data = new int[m_size];
        std::copy(r.m_data, r.m_data + m_size, m_data);
    }

    Resource(Resource &&r)
    {
        std::cout << "Resource move constructor" << std::endl;
        m_size = r.m_size;
        m_data = r.m_data;
        r.m_size = 0;
        r.m_data = nullptr;
    }

    Resource &operator=(Resource r)
    {
        std::cout << "Resource assignment operator" << std::endl;

        swap(*this, r);

        return *this;
    }

    int &operator[](int index)
    {
        return m_data[index];
    }

    void print()
    {
        std::cout << "Resource data: ";
        for (int i = 0; i < m_size; ++i)
        {
            std::cout << m_data[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    int m_size{};
    int *m_data{};

    void swap(Resource &a, Resource &b)
    {
        std::swap(a.m_size, b.m_size);
        std::swap(a.m_data, b.m_data);
    }
};

int main()
{
    Resource r1(10);

    for (int i = 0; i < 10; ++i)
    {
        r1[i] = i;
    }

    Resource r2;
    r2 = r1;

    r1.print();

    return 0;
} // end main
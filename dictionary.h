#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <string>
#include <vector>
#include <fstream>
#include <string.h>
using namespace std;

template <typename K>
class dictionary
{
    public:
        dictionary();
        dictionary(const dictionary &other);
        ~dictionary();
        int clear();
        int size() const;
        int input_from_file(const string &name);
        int output_to_file(const string &name) const;
        int operator==(const dictionary &other);
        int operator!=(const dictionary &other);
        void operator<<(const K &key);
        int delete_word(const K &key);
        int in_dictionary(const K &key) const;
        void insert(const K &key, int count);
        int hash(const K &key) const;
        int get_count(const K &key) const;
        int operator[](const K &key){ return get_count(key);}
        int count(int number) const;
        const vector<vector<pair<K, int>>> get() const
        {return m_map;}
    private:
        vector<vector<pair<K, int>>> m_map = vector<vector<pair<K, int>>>(512);
};

template<typename K>
dictionary<K>::dictionary()
{
    for(auto &i: this->m_map)
        for(auto &j: i)
            j.second = -1;
}


template<typename K>
dictionary<K>::dictionary(const dictionary &other)
{

    for(auto &i: other.m_map)
        for(auto &j: i)
            this->insert(j.first, j.second);
}


template<typename K>
dictionary<K>::~dictionary()
{
    for(auto &i: this->m_map)
        i.clear();
}


template<typename K>
int dictionary<K>::hash(const K &key) const
{
    int count = 0;
    for(const auto i: key)
        count += i;
    return count % 512;
}

template<typename K>
void dictionary<K>::insert(const K &key, int count)
{
    if (count >= 0)
    {
        auto index = hash(key);
        auto& vector = this->m_map[index];
        for (auto& c: vector)
            if (c.first == key)
            {
                c.second = count;
                return;
            }
        vector.push_back({key, count});
    }
}


template<typename K>
int dictionary<K>::get_count(const K &key) const
{
    auto index = hash(key);
    auto& vector = this->m_map[index];
    for (auto& c:vector)
        if (c.first == key)
            return c.second;
    return -1;
}

template<typename K>
int dictionary<K>::size() const
{
    int count = 0;

    for(auto &i: this->m_map)
        for(auto &j: i)
            if(j.second != -1)
                count++;
    return (count);

}

template<typename K>
int dictionary<K>::clear()
{
    for(auto &i: this->m_map)
        for(auto &j: i)
        {
            j.first = "\0";
            j.second = -1;
        }
    return 1;
}

template<typename K>
int dictionary<K>::input_from_file(const string &name)
{
    string word;
    int count;
    ifstream file(name);

    if(this->size() > 0)
        this->clear();
    if (file.is_open())
    {
        while(!file.eof())
        {
            file >> word;
            file >> count;
            if (word != "\0" && count > 0)
                this->insert(word, count);
        }
        file.close();
    }

    return 1;
}

template<typename K>
int dictionary<K>::output_to_file(const string &name) const
{
    ofstream fout(name);
    int number = 0;
    for(auto &i: this->m_map)
        for(auto &j: i)
        {
            if (j.second != -1 && j.first != "\0")
            {
                fout << j.first << endl;
                fout << j.second << "\n\n";
            }
        }

    fout.close();
    return 1;
}

template<typename K>
int dictionary<K>::operator==(const dictionary &other)
{
    if(this->m_map != other.m_map)
        return 0;
    return 1;
}

template<typename K>
void dictionary<K>::operator<<(const K &key)
{
    auto index = hash(key);
    auto& vector = this->m_map[index];
    for (auto& c: vector)
        if (c.first == key)
        {
            c.second++;
            return;
        }
    vector.push_back({key, 1});
}

template<typename K>
int dictionary<K>::delete_word(const K &key)
{
    auto index = hash(key);
    auto& vector = this->m_map[index];
    int i = 0;
    for (auto& c:vector)
    {
        if (c.first == key)
            vector.erase(vector.begin() + i);
        i++;
    }
    return 1;
}

template<typename K>
int dictionary<K>::in_dictionary(const K &key) const
{
    auto index = hash(key);
    auto& vector = this->m_map[index];
    for (auto& c:vector)
        if (c.first == key)
            return 1;
    return 0;
}

template<typename K>
int dictionary<K>::count(int number) const
{
    int out = 0;
    for(auto &i: this->m_map)
        for(auto &j: i)
            if(j.second >= number && j.second != -1)
                out++;
    return out;
}

template<typename K>
int dictionary<K>::operator!=(const dictionary &other)
{
    if(this->m_map == other.m_map)
        return 0;
    return 1;
}

#endif // DICTIONARY_H

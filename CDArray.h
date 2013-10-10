#ifndef CDARRAY_H
#define CDARRAY_H
/*
 * Заданее будет следующее:
 * реализовать недостающие функции нижеописанного класса (оставил кой какие функции
 * чтобы понятнее было, память выделять под p_Data динамически) и привести пример
 * его использования, скажем, заполнения 700 элементов случайными числами, в диапазоне
 * заданном пользователем, с последующей сортировкой(метод сортировки любой).
*/
template< class T >
class CDArray
{
    T *m_pData;
    int m_Number;
    int m_Size;
public:
    CDArray() : m_Number( 0 ), m_Size ( 1 ), m_pData(NULL)
        {
        }
    CDArray( int size ) : m_Number( 0 ), m_Size ( size ), m_pData(NULL)
        {
        }
    virtual ~CDArray()
        {
        free (m_pData);
        }
    //Add new element in end of array
    //resize array if necessary (use exponential growth)
    int Add( const T &t )
        {
        *( m_pData + m_Number ) = t;
        m_Number++;
        if( m_Number == m_Size ) {
            m_pData = (T*) realloc( m_pData, sizeof( T ) * m_Size * 2 );
            assert(m_pData);
            m_Size *= 2;
            }
        return m_Number;
        }
    //delete a number of elements from the end of array
    int Delete( int number )
        {
        assert(m_Number <= number);
        m_Number -= (m_Number < number ? m_Number : number);
        }
    //set nth element value T
    //on correctoperation return 0;
    //if n is out of (0, size-1) return size of array
    int Modify( int n, const T &t )
        {
        if(n >= m_Number)
            return m_Number;
        *( m_pData + n ) = t;
        return 0;
        }
    //set new size to array (with reallocation)
    //all data over new size will be lost
    //set size and number to size;
    int Resize( int size )
        {
        m_Number = m_Size = size;
        m_pData = (T*) realloc( m_pData, sizeof( T ) * m_Size * 2 );
        assert(m_pData);
        }
    void Clear()    { m_Number = 0; }
    T &operator[]( int i ) const
        {
        assert(m_Number <= i);
        return *( m_pData + i );
        }
    int Number()    { return m_Number; }
    int Size()      { return m_Size;  }
    T *Ptr()        { return m_pData; }
};
#endif // CDARRAY_H

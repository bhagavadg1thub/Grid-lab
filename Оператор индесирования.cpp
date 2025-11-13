#include <cassert>

template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;
    
private:
    T* data;
    size_type y_size, x_size;

    // Вспомогательный класс для оператора []
    class Proxy {
    private:
        T* row;
        size_type size;
    public:
        Proxy(T* row, size_type size) : row(row), size(size) {}
        
        T& operator[](size_type x_idx) {
            return row[x_idx];
        }
        
        T operator[](size_type x_idx) const {
            return row[x_idx];
        }
    };

public:
    // 1. Конструктор с одним параметром 
    Grid(T const &t) : data(new T[1]), y_size(1), x_size(1) {
        data[0] = t;
    }
    
    // 2. Конструктор с двумя параметрами 
    Grid(size_type y_size, size_type x_size) 
    : data(new T[y_size * x_size]), y_size(y_size), x_size(x_size) {}
    
    // 3. Конструктор с тремя параметрами 
    Grid(size_type y_size, size_type x_size, T const &t) 
    : data(new T[y_size * x_size]), y_size(y_size), x_size(x_size) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
    }
    
    // Деструктор
    ~Grid() {
        delete[] data;
    }
    
    // Конструктор копирования
    Grid(Grid const &other) 
    : data(new T[other.y_size * other.x_size]), 
      y_size(other.y_size), x_size(other.x_size) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // Оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            delete[] data;
            
            y_size = other.y_size;
            x_size = other.x_size;
            data = new T[y_size * x_size];
            for (size_type i = 0; i < y_size * x_size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // Конструктор перемещения
    Grid(Grid &&other) 
    : data(other.data), y_size(other.y_size), x_size(other.x_size) {
        other.data = nullptr;
        other.y_size = 0;
        other.x_size = 0;
    }
    
    // Оператор присваивания перемещением
    Grid& operator=(Grid &&other) {
        if (this != &other) {
            delete[] data;
            
            data = other.data;
            y_size = other.y_size;
            x_size = other.x_size;
            
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }
        return *this;
    }
    
    // Методы доступа к размерам (из исходного кода)
    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
    
    // Операторы доступа к элементам через круглые скобки (из исходного кода)
    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }
    
    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }
    
    // Оператор присваивания для заполнения всех элементов (из исходного кода)
    Grid<T>& operator=(T const &t) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
        return *this;
    }
    
    // Оператор индексирования [] для двойного доступа
    Proxy operator[](size_type y_idx) {
        return Proxy(&data[y_idx * x_size], x_size);
    }
    
    const Proxy operator[](size_type y_idx) const {
        return Proxy(&data[y_idx * x_size], x_size);
    }
};

// Тестовый код 
int main() {
    Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());
    
    using gsize_t = Grid<float>::size_type;
    
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);
    
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;
    
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(1.0f == g(y_idx, x_idx));
    
    return 0;
}

template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;
    
private:
    T* data;           // Указатель на массив
    size_type y_size;  // Количество строк
    size_type x_size;  // Количество столбцов

public:
    // 1. Конструктор с одним параметром (сетка 1x1)
    Grid(T const &t) : data(new T[1]), y_size(1), x_size(1) {
        data[0] = t;
    }
    
    // 2. Конструктор с двумя параметрами (размеры)
    Grid(size_type y_size, size_type x_size) 
    : data(new T[y_size * x_size]), y_size(y_size), x_size(x_size) {
        // Элементы инициализируются конструктором по умолчанию
    }
    
    // 3. Конструктор с тремя параметрами (размеры + значение)
    Grid(size_type y_size, size_type x_size, T const &t) 
    : data(new T[y_size * x_size]), y_size(y_size), x_size(x_size) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
    }
    
    // Деструктор - освобождает память
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
    
    // Методы доступа к размерам
    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
    
    // Операторы доступа к элементам через круглые скобки
    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }
    
    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }
    
    // Оператор присваивания для заполнения всех элементов
    Grid<T>& operator=(T const &t) {
        for (size_type i = 0; i < y_size * x_size; ++i) {
            data[i] = t;
        }
        return *this;
    }
};

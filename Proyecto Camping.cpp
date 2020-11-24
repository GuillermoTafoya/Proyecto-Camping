#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <assert.h>
#include <string>

//Borra la terminal
void flush() {
    system("CLS");
}

//Guarda el vector de productos en el archivo
void save() {
    std::cout << "Guardando....\n";
    //Falta trabajo jeje
}

class producto {
    public:
        int stock, id;
        std::string name, caducidad;

        //El constructor que le define los atributos del objeto.
        producto(std::string NAME = "", std::string CADUCIDAD = "", int STOCK = 0, int ID = 0) : name(NAME), stock(STOCK), caducidad(CADUCIDAD), id(ID){}

        void describe() { //Muestra los atributos del objeto
            std::cout << std::endl << std::setw(30) << std::left << id << std::setw(30) << name << std::setw(30) << stock << caducidad << std::endl;
        }
        
        bool operator==(const producto & obj)
        {
            return (stock == obj.stock) && (name == obj.name) && (caducidad == obj.caducidad) && (id == obj.id);
        }
        
        friend std::ostream& operator << (std::ostream& out, const producto& obj) //Define una acción a seguir cuando se utilize
        {                                                                         //un operador ostream en el objeto.
            out << std::endl << std::setw(30) << std::left << obj.id << std::setw(30) << obj.name << std::setw(30) << obj.stock << obj.caducidad;
            return out;
        }
       
        friend std::istream& operator >> (std::istream& in, producto& obj) //Define una acción a seguir cuando se utilize
        {                                                                  //un operador istream en el objeto.
            in >> obj.id;
            in >> obj.name;
            in >> obj.stock;
            in >> obj.caducidad;
            return in;
        }
};


//Añade productos al archivo.
void add() {
    flush();
    std::string name, caducidad, centinela;
    int stock;
    std::ifstream file;
    file.open("inventario.txt", std::ios::app);
    
    //Obtiene la llave primaria
    int id = -1;
    std::string line;
    while (std::getline(file, line))
        id++;
    file.close();

    std::ofstream file_obj;
    file_obj.open("inventario.txt", std::ios::app);

    while (true) {

        std::cout << "Para dejar de ingresar productos, escriba '-1' en cualquier momento.\n";
        std::cout << "Ingrese el nombre del producto (no usar espacios): ";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        //Valida el cin del nombre
        std::getline(std::cin, name);
        while (name.find(' ') != std::string::npos){
        
            std::cout << "Ingrese el nombre del producto (no usar espacios): ";
            std::getline(std::cin, name);
        }
        if (name == "-1") {
            break;
        }
        std::cout << "Ingrese el stock del producto: ";
        std::cin >> stock;
        if (stock == -1){
            break;
        }
        std::cout << "Ingrese la fecha de caducidad del producto: ";
        std::cin >> caducidad;
        if (caducidad == "-1") {
            break;
        flush();
        }
        flush();
        producto nuevo(name, caducidad, stock, id);

        id++;
        file_obj << nuevo;
    }

    file_obj.close();
}

//Elimina elementos del vector que contiene los productos y posteriormente guarda la versión modificada al archivo.
void obliterate() {
    std::cout << "Baja.\n";
    save();
}

//Modifica elementos del vector que contiene los productos y posteriormente guarda la versión modificada al archivo.
void change() {
    std::cout << "Cambio.\n";
    save();
}


//Crea un vector de los productos para guardar temporalmente la información en la RAM y facilitar su manipulación.
std::vector<producto> load() {
    std::vector<producto> inventario;
    std::string helper;
    std::ifstream file_obj("inventario.txt");
    producto obj;

    std::getline(file_obj, helper);

    int i = 0;

    while (!file_obj.eof()) {
        assert(file_obj >> obj);
        inventario.push_back(obj);
    }

    file_obj.close();

    return inventario;
}

//Muestra los objetos como una tabla
void describe(std::vector<producto> inventario) {

    flush();

    if (inventario.empty()) {
        std::cout << "No hay nada en el inventario.\n";
    }
    else {
        std::cout << std::endl << std::setw(30) << std::left << "ID" << std::setw(30) << "Producto" << std::setw(30) << "Stock" << "Caducidad" << std::endl;
    }

    for (int i = 0; i < inventario.size(); i++) {
        inventario[i].describe();
    }
    std::cout << std::endl << "Presione ENTER para continuar...";
    std::cin.get();
    getchar();
}

//Comprueba si el archivo está vacio
bool is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int main()
{
    int action;
    std::ifstream file_obj("inventario.txt");
    if (is_empty(file_obj)){
       file_obj.close();
       std::ofstream file_obj("inventario.txt");
       file_obj << std::setw(30) << std::left << "ID" << std::setw(30) << "Producto" << std::setw(30) << "Stock" << "Caducidad";
    }
    file_obj.close();
    std::vector<producto> inventario = load();
    do {
        flush();
        std::cout << "Elija una de las siguientes opciones:\n"; 
        std::cout << "1: Dar de alta\n";
        std::cout << "2: Dar de baja\n";
        std::cout << "3: Hacer un cambio\n";
        std::cout << "4: Ver resumen\n";
        std::cout << "5: Salir\n";
        std::cin >> action;
        std::cin.clear();

        switch (action) {
        default:
            std::cout << "Comando no reconocido. Intentelo de nuevo.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << "Presione ENTER para continuar...";
            std::cin.get();
            break;
        case -1:
            
            inventario = load();
            std::cout << "Base de datos actualizada.\n";
            break;
        case 1:
            add();
            inventario = load();
            break;
        case 2:
            obliterate();
            break;
        case 3:
            change();
            break;
        case 4:
            describe(inventario);
            break;
        case 5:
            save();
            break;
        }

    } while (action != 5);

    std::cout << "Proceso terminado.\n";

    return 0;
}